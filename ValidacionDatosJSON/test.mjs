// test.mjs
const good = {
  id: 1,
  nombre: "Luis",
  email: "luis@example.com",
  edad: 30,
  activo: true,
  roles: ["user"],
  direccion: { calle: "Av siempre viva", codigoPostal: "12345" }
};
const bad = {
  id: 0,
  nombre: "A",
  email: "no-email",
  roles: [],
  direccion: { codigoPostal: "12" },
  extra: "no permitido"
};

import { validateAjv } from "./validator-ajv.mjs";
import { validateZod } from "./validator-zod.mjs";
import { validateJoi } from "./validator-joi.mjs";

console.log("=== AJV ===");
console.log("Good:", validateAjv(good));
console.log("Bad:", validateAjv(bad));

console.log("\\n=== ZOD ===");
console.log("Good:", validateZod(good));
console.log("Bad:", validateZod(bad));

console.log("\\n=== JOI ===");
console.log("Good:", validateJoi(good));
console.log("Bad:", validateJoi(bad));
