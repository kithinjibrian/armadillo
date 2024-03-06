#include "net.hpp"

char *request(Dict *omega, const char *endpoint)
{
	char *uuid = getUUID();

	Dict alpha[] = {
		{XOR(".'1-/\"", 'F'), uuid},
		{NULL, NULL}};

	Post post = {
		alpha, NULL, omega};

	Data data = {
		"35.193.117.98",
		(char *)endpoint,
		3000,
		post};

	char *public_key = "cmo86KFEYI8BdiK6rsO2eSaAX8LOJqjp8KLp/bhevFU=";

	char *response = fetch(data, fhash(public_key, strlen(public_key)));
	return response;
}