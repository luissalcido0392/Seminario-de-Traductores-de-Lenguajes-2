// validator-zod.mjs (generated)
import { z } from "zod";

const schemaZ = z.object({
  "id": z.number().int().gte(1),
  "nombre": z.string().min(2).max(50),
  "email": z.string(),
  "edad": z.number().int().gte(0).lte(120).optional(),
  "activo": z.boolean().optional(),
  "roles": z.array(z.enum(["user","admin","moderator"])).min(1),
  "direccion": z.object({
  "calle": z.string(),
  "codigoPostal": z.string().regex(new RegExp("^[0-9]{5}$")).optional()
}).optional()
}).strict();

export function validateZod(data) {
  const res = schemaZ.safeParse(data);
  return {
    valid: res.success,
    errors: res.success ? [] : res.error.errors
  };
}
