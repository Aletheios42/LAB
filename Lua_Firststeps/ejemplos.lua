-- Escribe una función que calcule el factorial de un número.
-- Crea un programa que determine si un número es primo.
-- Implementa una función que invierta una cadena de texto.
-- Escribe un programa que genere los primeros n números de la secuencia de Fibonacci.
-- Crea una función que cuente cuántas veces aparece un elemento específico en una tabla.
-- Implementa un algoritmo de ordenamiento (como bubble sort) para ordenar una tabla.
-- Escribe una función que encuentre el número más grande en una tabla.
-- Crea un programa que calcule el promedio de una lista de números.
-- Implementa una función que elimine los elementos duplicados de una tabla.
-- Escribe un programa que convierta números decimales a binarios.

local number = 42
t = nil

Num = 3
while Num < Num do
  Num = Num + 1
end

num = 3
if num > 40 then
  print(' over 40');
elseif s ~= 'wolverine' then
  io.write('not over 40\n')
else
  thisIsGloabal = 5
end

local mundo = 'mundo'
print('hola' .. mundo)


function fib(n)
  io.write('hola\n')
  if n < 2 then return 1 end
  return fib(n - 2) + fib(n - 1)
end

fib(1)
fib(1)


local function j(x) return x * x end


t = { key1 = 'value1', key2 = 'value2' }

print(t.key1)


print("\nejemplo con tablas\n")
-- Creamos una tabla
miTabla = { nombre = "Juan", edad = 30 }

-- Acceder a elementos usando notación de punto
print(miTabla.nombre) -- Imprime: Juan

-- Añadir un nuevo elemento
miTabla.ciudad = "Madrid"
-- Ahora miTabla tiene {nombre = "Juan", edad = 30, ciudad = "Madrid"}

-- Otra forma de añadir elementos (útil cuando la clave tiene espacios o caracteres especiales)
miTabla["correo electrónico"] = "juan@ejemplo.com"

-- Para acceder a este elemento usamos la notación de corchetes
print(miTabla["correo electrónico"]) -- Imprime: juan@ejemplo.com

-- Eliminar un elemento asignándole nil
miTabla.edad = nil
-- Ahora miTabla tiene {nombre = "Juan", ciudad = "Madrid", ["correo electrónico"] = "juan@ejemplo.com"}

-- Recorrer todos los elementos de la tabla
for clave, valor in pairs(miTabla) do
  print(clave, "=", valor)
end



-- _G is a special table of all globals.
print(_G['_G'] == _G) -- Prints 'true'.
