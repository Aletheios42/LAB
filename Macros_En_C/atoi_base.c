/*****************************************************************************
 * Archivo: ft_atoi_base.c
 * 
 * Breve descripción:
 *   - Función ft_atoi_base(const char *str, const char *base) que convierte
 *     'str' en un entero decimal, interpretando los dígitos según la cadena
 *     'base'. Acepta letras minúsculas o mayúsculas tanto en 'str' como en
 *     'base'. 
 *   - Si la base es inválida (menos de 2 caracteres, contiene '+'/'-'/' ' u 
 *     otros duplicados), devuelve 0. 
 *   - Se detiene al encontrar un carácter fuera de la base, igual que atoi.
 *   - No maneja de forma especial "0x" al inicio: si necesitas omitirlo,
 *     llámala con (str + 2) cuando detectes prefijo "0x".
 *
 *   - Incluye un main de pruebas si se compila con -DTEST_MAIN.
 *
 * Compilación para probar:
 *   gcc ft_atoi_base.c -DTEST_MAIN -o test_atoi_base && ./test_atoi_base
 *
 *****************************************************************************/

#include <stdio.h>
#include <ctype.h>   // toupper, isspace
#include <stdbool.h> // bool, true, false
#include <assert.h>  // assert

/*****************************************************************************
 * ft_atoi_base: Conversión a entero según 'base'
 *****************************************************************************/
int	ft_atoi_base(const char *str, const char *base)
{
	char	base_up[128];
	bool	used[256] = { false };
	long	result = 0;
	int		sign = 1;
	int		len = 0;
	int		i, j;

	/* 1. Copiamos 'base' en base_up, normalizando a mayúsculas */
	for (i = 0; base[i] && i < 127; i++)
	{
		/* Si la base contiene +, -, espacio => inválida */
		if (base[i] == '+' || base[i] == '-' || isspace((unsigned char)base[i]))
			return 0;
		base_up[i] = (char)toupper((unsigned char)base[i]);
	}
	base_up[i] = '\0';
	len = i;

	/* 2. La base debe tener al menos 2 caracteres */
	if (len < 2)
		return 0;

	/* 3. Revisar si hay caracteres repetidos en base_up */
	for (i = 0; i < len; i++)
	{
		unsigned char c = (unsigned char)base_up[i];
		if (used[c]) // repetido => inválida
			return 0;
		used[c] = true;
	}

	/* 4. Saltar espacios iniciales en 'str' */
	while (*str && isspace((unsigned char)*str))
		str++;

	/* 5. Manejo del signo */
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}

	/* 6. Convertir dígitos */
	while (*str)
	{
		unsigned char c = (unsigned char)toupper((unsigned char)*str);
		/* Buscar c en base_up */
		for (j = 0; j < len; j++)
		{
			if ((unsigned char)base_up[j] == c)
				break;
		}
		/* Si no se encontró, se detiene el parse */
		if (j == len)
			break;
		result = result * len + j;
		str++;
	}
	return (int)(result * sign);
}

/*****************************************************************************
 * MAIN DE PRUEBAS (solo se compila con -DTEST_MAIN)
 *****************************************************************************/
#ifdef TEST_MAIN
int main(void)
{
	/*
	 * Pruebas típicas y, especialmente, con prefijo "0x":
	 *   -> Recuerda: ft_atoi_base NO omite automáticamente "0x".
	 *   -> Si quieres parsear "0xFF", llamas con ft_atoi_base("FF", ...).
	 */

	/* Hex en mayúsculas */
	assert(ft_atoi_base("FF", "0123456789ABCDEF") == 255);
	/* Hex en minúsculas */
	assert(ft_atoi_base("ff", "0123456789abcdef") == 255);

	/* Ejemplo color FDF: "0xFFA0" => parseamos la parte tras "0x" */
	/* 0xFFA0 = (255 << 8) + 160 = 65440 */
	assert(ft_atoi_base("FFA0", "0123456789ABCDEF") == 65440);
	assert(ft_atoi_base("ffa0", "0123456789abcdef") == 65440);

	/* Signo */
	assert(ft_atoi_base("-Fa", "0123456789ABCDEF") == -(15 * 16 + 10)); // -250
	assert(ft_atoi_base("   -42", "0123456789") == -42);

	/* Se detiene si ve un carácter fuera de la base, como 'x' en decimal */
	assert(ft_atoi_base("123xx", "0123456789") == 123);

	/* Binario */
	assert(ft_atoi_base("1010", "01") == 10);
	assert(ft_atoi_base("-1010", "01") == -10);

	/* Octal: '17' => 1*8 + 7 = 15 */
	assert(ft_atoi_base("17", "01234567") == 15);

	/* Base inválida => 0 */
	assert(ft_atoi_base("1010", "0") == 0); // Long. < 2
	assert(ft_atoi_base("123", "0123456789+") == 0); // '+' prohibido en la base
	assert(ft_atoi_base("12 3", "0123456789") != 0); // Ojo, espacios en str sí se saltan

	printf("¡Todas las pruebas han pasado!\n");
	return 0;
}
#endif

