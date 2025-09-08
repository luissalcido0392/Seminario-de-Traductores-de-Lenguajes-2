// generator.mjs
import fs from "fs/promises";

const schemaRaw = await fs.readFile("schema.json", "utf8");
const schema = JSON.parse(schemaRaw);

/* 1) Ajv: simply wrap the schema */
const ajvCode = `// validator-ajv.mjs (generated)
import Ajv from "ajv";
import addFormats from "ajv-formats";
const ajv = new Ajv({ allErrors: true, strict: false });
addFormats(ajv);
const schema = ${JSON.stringify(schema, null, 2)};
const validate = ajv.compile(schema);
export function validateAjv(data) {
  const valid = validate(data);
  return { valid: Boolean(valid), errors: validate.errors || [] };
}
`;

/* 2) Simple converter to Zod */
function schemaToZod(s) {
  if (s == null) return "z.any()";

  if (s.enum) {
    return `z.enum(${JSON.stringify(s.enum)})`;
  }

  if (s.type === "string") {
    let out = "z.string()";
    if (s.minLength != null) out += `.min(${s.minLength})`;
    if (s.maxLength != null) out += `.max(${s.maxLength})`;
    if (s.pattern) out += `.regex(new RegExp(${JSON.stringify(s.pattern)}))`;
    return out;
  }
  if (s.type === "integer" || s.type === "number") {
    let out = s.type === "integer" ? "z.number().int()" : "z.number()";
    if (s.minimum != null) out += `.gte(${s.minimum})`;
    if (s.maximum != null) out += `.lte(${s.maximum})`;
    return out;
  }
  if (s.type === "boolean") return "z.boolean()";
  if (s.type === "array") {
    const items = schemaToZod(s.items || {});
    let out = `z.array(${items})`;
    if (s.minItems != null) out += `.min(${s.minItems})`;
    if (s.maxItems != null) out += `.max(${s.maxItems})`;
    return out;
  }
  if (s.type === "object" || s.properties) {
    const props = s.properties || {};
    const required = new Set(s.required || []);
    const entries = Object.entries(props).map(([k, v]) => {
      const z = schemaToZod(v);
      const maybe = required.has(k) ? z : `${z}.optional()`;
      return `  ${JSON.stringify(k)}: ${maybe}`;
    }).join(",\n");
    let out = `z.object({\n${entries}\n})`;
    if (s.additionalProperties === false) out += `.strict()`;
    return out;
  }
  return "z.any()";
}

const zodSchemaStr = schemaToZod(schema);

const zodCode = `// validator-zod.mjs (generated)
import { z } from "zod";

const schemaZ = ${zodSchemaStr};

export function validateZod(data) {
  const res = schemaZ.safeParse(data);
  return {
    valid: res.success,
    errors: res.success ? [] : res.error.errors
  };
}
`;

/* 3) Simple converter to Joi */
function schemaToJoi(s) {
  if (s == null) return "Joi.any()";
  if (s.enum) {
    return `Joi.valid(${s.enum.map(v => JSON.stringify(v)).join(", ")})`;
  }
  if (s.type === "string") {
    let out = "Joi.string()";
    if (s.minLength != null) out += `.min(${s.minLength})`;
    if (s.maxLength != null) out += `.max(${s.maxLength})`;
    if (s.pattern) out += `.pattern(new RegExp(${JSON.stringify(s.pattern)}))`;
    return out;
  }
  if (s.type === "integer" || s.type === "number") {
    let out = s.type === "integer" ? "Joi.number().integer()" : "Joi.number()";
    if (s.minimum != null) out += `.min(${s.minimum})`;
    if (s.maximum != null) out += `.max(${s.maximum})`;
    return out;
  }
  if (s.type === "boolean") return "Joi.boolean()";
  if (s.type === "array") {
    const items = schemaToJoi(s.items || {});
    let out = `Joi.array().items(${items})`;
    if (s.minItems != null) out += `.min(${s.minItems})`;
    if (s.maxItems != null) out += `.max(${s.maxItems})`;
    return out;
  }
  if (s.type === "object" || s.properties) {
    const props = s.properties || {};
    const required = new Set(s.required || []);
    const entries = Object.entries(props).map(([k, v]) => {
      const joi = schemaToJoi(v);
      const maybe = required.has(k) ? `${joi}.required()` : `${joi}.optional()`;
      return `  ${JSON.stringify(k)}: ${maybe}`;
    }).join(",\n");
    let out = `Joi.object({\n${entries}\n})`;
    if (s.additionalProperties === false) out += `.unknown(false)`;
    return out;
  }
  return "Joi.any()";
}

const joiSchemaStr = schemaToJoi(schema);

const joiCode = `// validator-joi.mjs (generated)
import Joi from "joi";

const schemaJ = ${joiSchemaStr};

export function validateJoi(data) {
  const { error, value } = schemaJ.validate(data, { abortEarly: false });
  return { valid: !error, errors: error ? error.details : [] };
}
`;

/* write files */
await fs.writeFile("validator-ajv.mjs", ajvCode);
await fs.writeFile("validator-zod.mjs", zodCode);
await fs.writeFile("validator-joi.mjs", joiCode);

console.log("Generated: validator-ajv.mjs, validator-zod.mjs, validator-joi.mjs");
