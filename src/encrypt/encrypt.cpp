#include "encrypt.hpp"

KeyIV genKeyIv()
{
	KeyIV keyiv;
	randombytes_buf(keyiv.key, KEY_LENGTH);
	randombytes_buf(keyiv.iv, IV_LENGTH);
	keyiv.keyLength = KEY_LENGTH;
	keyiv.ivLength = IV_LENGTH;
	return keyiv;
}

KeyIV *genIV(KeyIV *keyiv)
{
	randombytes_buf(keyiv->iv, IV_LENGTH);
	return keyiv;
}

char *XOR(char *data, char key)
{
	int dataLen = strlen(data);
	char *output = (char *)malloc(sizeof(char) * dataLen);

	for (int i = 0; i < dataLen; ++i)
	{
		output[i] = data[i] ^ key;
	}

	output[dataLen] = '\0';

	return output;
}
