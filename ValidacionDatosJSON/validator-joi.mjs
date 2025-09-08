// validator-joi.mjs (generated)
import Joi from "joi";

const schemaJ = Joi.object({
  "id": Joi.number().integer().min(1).required(),
  "nombre": Joi.string().min(2).max(50).required(),
  "email": Joi.string().required(),
  "edad": Joi.number().integer().min(0).max(120).optional(),
  "activo": Joi.boolean().optional(),
  "roles": Joi.array().items(Joi.valid("user", "admin", "moderator")).min(1).required(),
  "direccion": Joi.object({
  "calle": Joi.string().required(),
  "codigoPostal": Joi.string().pattern(new RegExp("^[0-9]{5}$")).optional()
}).optional()
}).unknown(false);

export function validateJoi(data) {
  const { error, value } = schemaJ.validate(data, { abortEarly: false });
  return { valid: !error, errors: error ? error.details : [] };
}
