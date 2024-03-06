#include "jfunc.hpp"

Command deserializeCommand(const char *json_string)
{

	cJSON *root = cJSON_Parse(json_string);
	Command command;

	if (root == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			// fprintf(stderr, "Error before: %s\n", error_ptr);
			command.id = NULL;
			return command;
		}
	}

	cJSON *id = cJSON_GetObjectItemCaseSensitive(root, "id");
	cJSON *cmd = cJSON_GetObjectItemCaseSensitive(root, "command");

	if (!(id && cJSON_IsString(id) &&
		  cmd && cJSON_IsString(cmd)))
	{
		// fprintf(stderr, "Error: Unable to extract values from JSON\n");
		cJSON_Delete(root);
		command.id = NULL;
		return command;
	}

	command.id = strdup(id->valuestring);
	command.command = strdup(cmd->valuestring);

	// cJSON_Delete(root);

	return command;
}
