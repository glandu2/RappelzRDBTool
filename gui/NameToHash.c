#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>

#include "NameToHash.h"

static char encryptLastCharTable[0x55] = "^&T_Nsd{xo5v`rOYV+,iIU#kCJq8$\'~L0P]FeBn-Au(pXHZhwDy2}agWG7K=bQ;SRt)46l@jE%9!c1[3fmMz";

static char encryptTablePhase2[0x80] =
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x67, 0x20, 0x00, 0x26, 0x77, 0x2C, 0x6C, 0x4E, 0x58, 0x4F, 0x00, 0x37, 0x2E, 0x25, 0x65, 0x00, 0x38, 0x5F, 0x5D, 0x23, 0x50, 0x31, 0x2D, 0x24, 0x56, 0x5B, 0x00, 0x59, 0x00, 0x5E, 0x00, 0x00,
 0x4B, 0x7D, 0x6A, 0x30, 0x40, 0x47, 0x53, 0x29, 0x41, 0x78, 0x79, 0x36, 0x39, 0x45, 0x46, 0x7B, 0x57, 0x62, 0x3D, 0x52, 0x76, 0x74, 0x68, 0x32, 0x34, 0x4D, 0x28, 0x6B, 0x00, 0x6D, 0x61, 0x2B,
 0x7E, 0x44, 0x27, 0x43, 0x21, 0x4A, 0x49, 0x64, 0x42, 0x55, 0x60, 0x71, 0x66, 0x70, 0x48, 0x51, 0x33, 0x4C, 0x6E, 0x6F, 0x5A, 0x69, 0X72, 0x73, 0x75, 0x3B, 0x7A, 0x63, 0x00, 0x54, 0x35, 0x00};


void prepareHash(char *hash, int size) {
	char val1, val2;
	int medianPt13 = (int)floor(0.33*size);
	int medianPt23 = (int)floor(0.66*size);

	val1 = hash[medianPt23];
	val2 = hash[medianPt13];

	hash[medianPt23] = hash[0];
	hash[medianPt13] = hash[1];

	hash[0] = val1;
	hash[1] = val2;
}

void encryptNameToHash(char *name, unsigned int size, unsigned int encodeSeed) {
	unsigned int i, j;
	unsigned char computeVar;
	unsigned int computeLoop = encodeSeed;

	for(i=0; i < size; i++) {
		computeVar = name[i];

		for(j=0; j < computeLoop; j++)
			computeVar = encryptTablePhase2[computeVar];

		computeLoop = (1+computeLoop + 17*name[i])&0x1F;
		if(!computeLoop) computeLoop = 0x20;
		name[i] = computeVar;
	}
}

int computeLegacySeed(const char *name) {
	int i, computeVar;

	for(i = 0, computeVar = 0; name[i]; i++)
		computeVar = name[i]*17 + computeVar + 1;

	computeVar = (i + computeVar) & 0x1F;
	if (!computeVar) return 0x20;
	else return computeVar;
}

char computeFirstChar(char *reducedHash, unsigned int size) {
	unsigned int i, computeVar;

	for(i = 0, computeVar = 0; i < size; i++)
		computeVar = reducedHash[i] + computeVar;

	return encryptLastCharTable[computeVar % 0x54];
}

/**
 * Converti un nom en son hash.
 * \param name Le nom a convertir
 * \param hash Un buffer qui contiendra le hash associé au nom, \warning la taille du buffer doit pouvoir stocker 2 caractères de plus que le nom !
 * \param encodeSeed Entier utilisé pour convertir le nom. Utilisez LEGACY_SEED pour que le hash soit compatible avec Rappelz. La valeur doit être supérieur ou égal a 0 ou égale a LEGACY_SEED.
 * \return Code de l'erreur:
 * \li \c 0 Pas d'erreur.
 * \li \c EINVAL hash ou name est NULL ou encodeSeed est négatif.
 * \sa convertHashToName
 */
int convertNameToHash(const char *name, char *hash, int encodeSeed) {
	unsigned int nameSize = strlen(name);
	char *reducedHash = alloca(nameSize);

	if(!name || !hash) return EINVAL;

	if(encodeSeed == LEGACY_SEED) encodeSeed = computeLegacySeed(name);
	if(encodeSeed < 0) return EINVAL;

	memcpy(reducedHash, name, nameSize);
	encryptNameToHash(reducedHash, nameSize, (unsigned int) encodeSeed);
	prepareHash(reducedHash, nameSize);
	memcpy(hash+1, reducedHash, nameSize);
	hash[nameSize+1] = encryptLastCharTable[encodeSeed];
	hash[nameSize+2] = 0;

	hash[0] = computeFirstChar(reducedHash, nameSize);

	return 0;
}

