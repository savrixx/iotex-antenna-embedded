#include "signer.h"
#include "pb_proto.h"
#include "iotex_emb.h"

#define SAFE_STRLEN(s) ((s) ? strlen(s) : 0)
#define GET_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/*
 * @brief: generate transfer action bytes
 * #tx: transfer action params
 * #action_bytes: store action bytes
 * #max_size: indicate #action_bytes max size in bytes
 * #private_key: account private key
 * $return: success retrun #action_bytes actual size, failed return negative error code
 */
int proto_gen_tx_action(const struct iotex_st_transfer *tx, uint8_t *action_bytes, size_t max_size, const char *private_key) {

    int pb_len;
    uint8_t action_signature[SIG_SIGNATURE_SIZE] = {0};
    uint8_t public_key_bytes[SIG_PUBLIC_KEY_SIZE] = {0};
    uint8_t private_key_bytes[SIG_PRIVATE_KEY_SIZE] = {0};

    /* Private key string to bytes */
    if (signer_str2hex(private_key, private_key_bytes, sizeof(private_key_bytes)) != SIG_PRIVATE_KEY_SIZE) {

        return -1;
    }

    /* Get public key from private key */
    signer_get_public_key(private_key_bytes, public_key_bytes);

    pb_st_item tx_msg[] = {

        {PB_WT_LD, TX_AMOUNT, (void *)tx->amount, SAFE_STRLEN(tx->amount)},
        {PB_WT_LD, TX_RECIPIENT, (void *)tx->recipient, SAFE_STRLEN(tx->recipient)},
        {PB_WT_LD, TX_PAYLOAD, (void *)tx->payload, tx->payloadLength},
    };

    pb_st_item act_core[] = {

        {PB_WT_VARINT, AC_VERSION, (void *)tx->version},
        {PB_WT_VARINT, AC_NONCE, (void *)tx->nonce},
        {PB_WT_VARINT, AC_GAS_LIMIT, (void *)tx->gasLimit},
        {PB_WT_LD, AC_GAS_PRICE, (void *)tx->gasPrice, SAFE_STRLEN(tx->gasPrice)},
        {PB_WT_EMB, ACT_TRANSFER, (void *)tx_msg, GET_ARRAY_SIZE(tx_msg)},
    };

    pb_st_item tx_action[] = {
        {PB_WT_EMB, ACTION_CORE, (void *)act_core, GET_ARRAY_SIZE(act_core)},
        {PB_WT_LD, ACTION_SND_PUBKEY, (void *)public_key_bytes, sizeof(public_key_bytes)},
        {PB_WT_LD, ACTION_SIGNATURE, (void *)action_signature, sizeof(action_signature)},
    };

    /* Pack act_core, calc hash, generate action signature */
    if ((pb_len = pb_pack(act_core, GET_ARRAY_SIZE(act_core), action_bytes, max_size)) < 0) {

        return pb_len;
    }

    if (signer_get_signature(action_bytes, pb_len, private_key_bytes, action_signature) != 0) {

        return -1;
    }

    return pb_pack(tx_action, GET_ARRAY_SIZE(tx_action), action_bytes, max_size);
}
