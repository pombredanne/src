/* crypto/evp/evp_err.c */
/* ====================================================================
 * Copyright (c) 1999-2005 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

/* NOTE: this file was auto generated by the mkerr.pl script: any changes
 * made to it will be overwritten when the script next updates this file,
 * only reason strings will be preserved.
 */

#include <stdio.h>
#include <openssl/err.h>
#include <openssl/evp.h>

/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR
static ERR_STRING_DATA EVP_str_functs[]=
	{
{ERR_PACK(0,EVP_F_AES_INIT_KEY,0),	"AES_INIT_KEY"},
{ERR_PACK(0,EVP_F_D2I_PKEY,0),	"D2I_PKEY"},
{ERR_PACK(0,EVP_F_EVP_ADD_CIPHER,0),	"EVP_add_cipher"},
{ERR_PACK(0,EVP_F_EVP_ADD_DIGEST,0),	"EVP_add_digest"},
{ERR_PACK(0,EVP_F_EVP_CIPHERINIT,0),	"EVP_CipherInit"},
{ERR_PACK(0,EVP_F_EVP_CIPHER_CTX_CTRL,0),	"EVP_CIPHER_CTX_ctrl"},
{ERR_PACK(0,EVP_F_EVP_CIPHER_CTX_SET_KEY_LENGTH,0),	"EVP_CIPHER_CTX_set_key_length"},
{ERR_PACK(0,EVP_F_EVP_DECRYPTFINAL,0),	"EVP_DecryptFinal"},
{ERR_PACK(0,EVP_F_EVP_DIGESTINIT,0),	"EVP_DigestInit"},
{ERR_PACK(0,EVP_F_EVP_ENCRYPTFINAL,0),	"EVP_EncryptFinal"},
{ERR_PACK(0,EVP_F_EVP_GET_CIPHERBYNAME,0),	"EVP_get_cipherbyname"},
{ERR_PACK(0,EVP_F_EVP_GET_DIGESTBYNAME,0),	"EVP_get_digestbyname"},
{ERR_PACK(0,EVP_F_EVP_MD_CTX_COPY,0),	"EVP_MD_CTX_copy"},
{ERR_PACK(0,EVP_F_EVP_OPENINIT,0),	"EVP_OpenInit"},
{ERR_PACK(0,EVP_F_EVP_PBE_ALG_ADD,0),	"EVP_PBE_alg_add"},
{ERR_PACK(0,EVP_F_EVP_PBE_CIPHERINIT,0),	"EVP_PBE_CipherInit"},
{ERR_PACK(0,EVP_F_EVP_PKCS82PKEY,0),	"EVP_PKCS82PKEY"},
{ERR_PACK(0,EVP_F_EVP_PKCS8_SET_BROKEN,0),	"EVP_PKCS8_SET_BROKEN"},
{ERR_PACK(0,EVP_F_EVP_PKEY2PKCS8,0),	"EVP_PKEY2PKCS8"},
{ERR_PACK(0,EVP_F_EVP_PKEY_COPY_PARAMETERS,0),	"EVP_PKEY_copy_parameters"},
{ERR_PACK(0,EVP_F_EVP_PKEY_DECRYPT,0),	"EVP_PKEY_decrypt"},
{ERR_PACK(0,EVP_F_EVP_PKEY_ENCRYPT,0),	"EVP_PKEY_encrypt"},
{ERR_PACK(0,EVP_F_EVP_PKEY_GET1_DH,0),	"EVP_PKEY_get1_DH"},
{ERR_PACK(0,EVP_F_EVP_PKEY_GET1_DSA,0),	"EVP_PKEY_get1_DSA"},
{ERR_PACK(0,EVP_F_EVP_PKEY_GET1_RSA,0),	"EVP_PKEY_get1_RSA"},
{ERR_PACK(0,EVP_F_EVP_PKEY_NEW,0),	"EVP_PKEY_new"},
{ERR_PACK(0,EVP_F_EVP_RIJNDAEL,0),	"EVP_RIJNDAEL"},
{ERR_PACK(0,EVP_F_EVP_SIGNFINAL,0),	"EVP_SignFinal"},
{ERR_PACK(0,EVP_F_EVP_VERIFYFINAL,0),	"EVP_VerifyFinal"},
{ERR_PACK(0,EVP_F_PKCS5_PBE_KEYIVGEN,0),	"PKCS5_PBE_keyivgen"},
{ERR_PACK(0,EVP_F_PKCS5_V2_PBE_KEYIVGEN,0),	"PKCS5_v2_PBE_keyivgen"},
{ERR_PACK(0,EVP_F_RC2_MAGIC_TO_METH,0),	"RC2_MAGIC_TO_METH"},
{ERR_PACK(0,EVP_F_RC5_CTRL,0),	"RC5_CTRL"},
{0,NULL}
	};

static ERR_STRING_DATA EVP_str_reasons[]=
	{
{EVP_R_AES_KEY_SETUP_FAILED              ,"aes key setup failed"},
{EVP_R_BAD_BLOCK_LENGTH                  ,"bad block length"},
{EVP_R_BAD_DECRYPT                       ,"bad decrypt"},
{EVP_R_BAD_KEY_LENGTH                    ,"bad key length"},
{EVP_R_BN_DECODE_ERROR                   ,"bn decode error"},
{EVP_R_BN_PUBKEY_ERROR                   ,"bn pubkey error"},
{EVP_R_CIPHER_PARAMETER_ERROR            ,"cipher parameter error"},
{EVP_R_CTRL_NOT_IMPLEMENTED              ,"ctrl not implemented"},
{EVP_R_CTRL_OPERATION_NOT_IMPLEMENTED    ,"ctrl operation not implemented"},
{EVP_R_DATA_NOT_MULTIPLE_OF_BLOCK_LENGTH ,"data not multiple of block length"},
{EVP_R_DECODE_ERROR                      ,"decode error"},
{EVP_R_DIFFERENT_KEY_TYPES               ,"different key types"},
{EVP_R_DISABLED_FOR_FIPS                 ,"disabled for fips"},
{EVP_R_ENCODE_ERROR                      ,"encode error"},
{EVP_R_EVP_PBE_CIPHERINIT_ERROR          ,"evp pbe cipherinit error"},
{EVP_R_EXPECTING_AN_RSA_KEY              ,"expecting an rsa key"},
{EVP_R_EXPECTING_A_DH_KEY                ,"expecting a dh key"},
{EVP_R_EXPECTING_A_DSA_KEY               ,"expecting a dsa key"},
{EVP_R_INITIALIZATION_ERROR              ,"initialization error"},
{EVP_R_INPUT_NOT_INITIALIZED             ,"input not initialized"},
{EVP_R_INVALID_KEY_LENGTH                ,"invalid key length"},
{EVP_R_IV_TOO_LARGE                      ,"iv too large"},
{EVP_R_KEYGEN_FAILURE                    ,"keygen failure"},
{EVP_R_MISSING_PARAMETERS                ,"missing parameters"},
{EVP_R_NO_CIPHER_SET                     ,"no cipher set"},
{EVP_R_NO_DIGEST_SET                     ,"no digest set"},
{EVP_R_NO_DSA_PARAMETERS                 ,"no dsa parameters"},
{EVP_R_NO_SIGN_FUNCTION_CONFIGURED       ,"no sign function configured"},
{EVP_R_NO_VERIFY_FUNCTION_CONFIGURED     ,"no verify function configured"},
{EVP_R_PKCS8_UNKNOWN_BROKEN_TYPE         ,"pkcs8 unknown broken type"},
{EVP_R_PUBLIC_KEY_NOT_RSA                ,"public key not rsa"},
{EVP_R_UNKNOWN_PBE_ALGORITHM             ,"unknown pbe algorithm"},
{EVP_R_UNSUPORTED_NUMBER_OF_ROUNDS       ,"unsuported number of rounds"},
{EVP_R_UNSUPPORTED_CIPHER                ,"unsupported cipher"},
{EVP_R_UNSUPPORTED_KEYLENGTH             ,"unsupported keylength"},
{EVP_R_UNSUPPORTED_KEY_DERIVATION_FUNCTION,"unsupported key derivation function"},
{EVP_R_UNSUPPORTED_KEY_SIZE              ,"unsupported key size"},
{EVP_R_UNSUPPORTED_PRF                   ,"unsupported prf"},
{EVP_R_UNSUPPORTED_PRIVATE_KEY_ALGORITHM ,"unsupported private key algorithm"},
{EVP_R_UNSUPPORTED_SALT_TYPE             ,"unsupported salt type"},
{EVP_R_WRONG_FINAL_BLOCK_LENGTH          ,"wrong final block length"},
{EVP_R_WRONG_PUBLIC_KEY_TYPE             ,"wrong public key type"},
{0,NULL}
	};

#endif

void ERR_load_EVP_strings(void)
	{
	static int init=1;

	if (init)
		{
		init=0;
#ifndef OPENSSL_NO_ERR
		ERR_load_strings(ERR_LIB_EVP,EVP_str_functs);
		ERR_load_strings(ERR_LIB_EVP,EVP_str_reasons);
#endif

		}
	}
