#include <stdio.h>
#include "unittest.h"
#include "test_config.h"
#include "../src/debug.h"
#include "../src/signer.h"
#include "../src/pb_pack.h"


void test_pack_varint() {

    size_t i;
    int pb_len, hex_len;
    uint8_t pb_buffer[256];
    char serialize_hex[256];

    pb_st_item multi[6];
    pb_st_item single = {PB_WT_VARINT, 1};
    uint64_t datas[] = {1, 300, 0x123, 0x1f2f32, 0x13232323, 0x3214325412};
    const char *serialize_str[] = {

        "0801",
        "08ac02",
        "08a302",
        "08b2de7c",
        "08a3c68c9901",
        "0892a8c9a1a106",
    };


    for (i = 0; i < UNITTEST_GET_ARRAY_SIZE(datas); i++) {

        multi[i].type = PB_WT_VARINT;
        multi[i].field = i + 1;
        multi[i].data = (void *)&datas[i];

        single.data = (void *)&datas[i];
        pb_len = pb_pack(&single, 1, pb_buffer, sizeof(pb_buffer));

        UNITTEST_ASSERT_NE(-1, pb_len);
        UNITTEST_ASSERT_NE(-1, signer_hex(pb_buffer, pb_len, serialize_hex, sizeof(serialize_hex)));
        UNITTEST_ASSERT_STR_EQ(serialize_hex, serialize_str[i], strlen(serialize_str[i]));
    }


    pb_len = pb_pack(multi, UNITTEST_GET_ARRAY_SIZE(multi), pb_buffer, sizeof(pb_buffer));
    hex_len = signer_hex(pb_buffer, pb_len, serialize_hex, sizeof(serialize_hex));

    UNITTEST_ASSERT_NE(-1, pb_len);
    UNITTEST_ASSERT_NE(-1, hex_len);
    UNITTEST_ASSERT_STR_EQ(serialize_hex, "080110ac0218a30220b2de7c28a3c68c99013092a8c9a1a106", hex_len);

    UNITTEST_AUTO_PASS();
}


int main(int argc, char **argv) {


    test_pack_varint();

    return 0;
}
