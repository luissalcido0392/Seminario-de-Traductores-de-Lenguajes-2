// validator-ajv.mjs (generated)
import Ajv from "ajv";
import addFormats from "ajv-formats";
const ajv = new Ajv({ allErrors: true, strict: false });
addFormats(ajv);
const schema = {
  "$id": "https://ejemplo.local/persona.schema.json",
  "type": "object",
  "properties": {
    "id": {
      "type": "integer",
      "minimum": 1
    },
    "nombre": {
      "type": "string",
      "minLength": 2,
      "maxLength": 50
    },
    "email": {
      "type": "string",
      "format": "email"
    },
    "edad": {
      "type": "integer",
      "minimum": 0,
      "maximum": 120
    },
    "activo": {
      "type": "boolean"
    },
    "roles": {
      "type": "array",
      "items": {
        "type": "string",
        "enum": [
          "user",
          "admin",
          "moderator"
        ]
      },
      "minItems": 1
    },
    "direccion": {
      "type": "object",
      "properties": {
        "calle": {
          "type": "string"
        },
        "codigoPostal": {
          "type": "string",
          "pattern": "^[0-9]{5}$"
        }
      },
      "required": [
        "calle"
      ]
    }
  },
  "required": [
    "id",
    "nombre",
    "email",
    "roles"
  ],
  "additionalProperties": false
};
const validate = ajv.compile(schema);
export function validateAjv(data) {
  const valid = validate(data);
  return { valid: Boolean(valid), errors: validate.errors || [] };
}
