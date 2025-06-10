#ifndef SECURE_LINK_H_
#define SECURE_LINK_H_

// secure_link.h
#ifndef SECURE_LINK_H_
#define SECURE_LINK_H_

#include <stddef.h>
#include <stdint.h>

// Inicializa AES y Ethernet HAL
void SecureLink_Init(void);

// Cifra `plain` de longitud `len`, arma un frame y lo envía.
// Devuelve true si tuvo éxito.
bool SecureLink_Send(const uint8_t *plain, size_t len);

// Intenta leer un frame, descifrarlo y escribir el payload en `out`.
// `out_len` es la longitud de `out` y queda ajustada a la longitud descifrada.
// Devuelve true si llegó un mensaje válido.
bool SecureLink_Receive(uint8_t *out, size_t *out_len);

#endif // SECURE_LINK_H_


#endif /* SECURE_LINK_H_ */
