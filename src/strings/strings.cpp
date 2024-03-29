#include "strings.hpp"

char *stitch(int numStrings, ...)
{
	va_list args;
	int totalLength = 0;
	char *result, *str;

	// Calculate total length of all strings
	va_start(args, numStrings);
	for (int i = 0; i < numStrings; i++)
	{
		str = va_arg(args, char *);
		totalLength += strlen(str);
	}
	va_end(args);

	// Allocate memory for the concatenated char *
	result = (char *)malloc(totalLength + 1); // +1 for the null terminator
	if (!result)
	{
		// printf("Memory allocation failed.\n");
		return NULL;
	}

	// Concatenate strings
	va_start(args, numStrings);
	result[0] = '\0'; // Ensure the result is an empty char * initially
	for (int i = 0; i < numStrings; i++)
	{
		str = va_arg(args, char *);
		strcat(result, str);
	}
	va_end(args);

	return result;
}

int token_len(char *str, char *delim)
{
	int index = 0, len = 0;

	while (*(str + index) && *(str + index) != *delim)
	{
		len++;
		index++;
	}

	return (len);
}

int count_tokens(char *str, char *delim)
{
	int index, tokens = 0, len = 0;

	for (index = 0; *(str + index); index++)
		len++;

	for (index = 0; index < len; index++)
	{
		if (*(str + index) != *delim)
		{
			tokens++;
			index += token_len(str + index, delim);
		}
	}

	return (tokens);
}

char **str_tok(char *line, char *delim)
{
	char **ptr;
	int index = 0, tokens, t, letters, l;

	tokens = count_tokens(line, delim);
	if (tokens == 0)
		return (NULL);

	ptr = (char **)malloc(sizeof(char *) * (tokens + 2));
	if (!ptr)
		return (NULL);

	for (t = 0; t < tokens; t++)
	{
		while (line[index] == *delim)
			index++;

		letters = token_len(line + index, delim);

		ptr[t] = (char *)malloc(sizeof(char) * (letters + 1));
		if (!ptr[t])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			return (NULL);
		}

		for (l = 0; l < letters; l++)
		{
			ptr[t][l] = line[index];
			index++;
		}

		ptr[t][l] = '\0';
	}
	ptr[t] = NULL;
	ptr[t + 1] = NULL;

	return (ptr);
}

char **split(char *line, char *delimeter)
{
	return str_tok(line, delimeter);
}
