## Interact with pebble contract 

Pebble contract is deployed on mainnet to administer pebble device registration and subscription. 

You can use a device's ID to read its order information, and send IoT data to the subscriber's storage endpoint.

```c
const char *pebble = "io1zclqa7w3gxpk47t3y3g9gzujgtl44lastfth28"; // to be replaced by actual contract address
const char *method = "c5934222";
const char *data = "0000000000000000000000000000000000333532363536313030373934363132"; // hex-encoded string of 32-byte device ID
iotex_st_contract_data contract_data = {};

// read the device's order info from contract
if (iotex_emb_read_contract_by_addr(pebble, method, data, &contract_data) != 0) {
    // error handling
}

// parse order info
uint64_t duration = abi_get_order_duration(contract_data.data); // number of blocks subscriber paid to receive data
const char *endpoint = abi_get_order_endpoint(contract_data.data); // subscriber's storage endpoint address
const char *token = abi_get_order_token(contract_data.data); // subscriber's storage endpoint token

// device should start sending IoT data to the subscriber's storage endpoint for duration * 5 seconds
```
