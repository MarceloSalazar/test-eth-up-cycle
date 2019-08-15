# Ethernet bring-up reliability Test

### Setup project and run test

```
mbed import ...
cd test-eth-up-cycle
mbed test -t <toolchain> -m <platform> -n *eth_up* -v -DTESTCASE_COUNT=100
```

Note, if `TESTCASE_COUNT` is not specified, the default value is 100.

### Expected test results

```
mbedgt: test case report:
| target              | platform_name | test suite         | test case   | passed | failed | result | elapsed_time (sec) |
|---------------------|---------------|--------------------|-------------|--------|--------|--------|--------------------|
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   0 | 1      | 0      | OK     | 3.68               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   1 | 1      | 0      | OK     | 2.69               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   2 | 1      | 0      | OK     | 4.2                |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   3 | 1      | 0      | OK     | 2.68               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   4 | 1      | 0      | OK     | 4.19               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   5 | 1      | 0      | OK     | 2.68               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   6 | 1      | 0      | OK     | 4.69               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   7 | 1      | 0      | OK     | 2.69               |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   8 | 1      | 0      | OK     | 4.7                |
| NUCLEO_F429ZI-ARMC6 | NUCLEO_F429ZI | tests-basic-eth_up | Connect   9 | 1      | 0      | OK     | 2.68               |
mbedgt: test case results: 0 FAIL / 10 OK
mbedgt: completed in 172.70 sec
mbedgt: exited with code 1
```
