README - Schema Translator

Instrucciones rápidas:
1. npm install
2. npm run generate   # optional - regenerates validators from schema.json
3. npm run test

Archivos incluidos:
- schema.json : ejemplo de JSON Schema
- generator.mjs : script que convierte schema.json a validadores (Ajv, Zod, Joi)
- validator-ajv.mjs, validator-zod.mjs, validator-joi.mjs : archivos generados
- test.mjs : pruebas de ejemplo
