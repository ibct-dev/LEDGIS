
# LEDGIS - LEDGIS blockchain infrastructure

#### LEDGIS Setting

```sh
$ git clone https://github.com/ibct-dev/LEDGIS --recursive
$ cd LEDGIS
$ sudo ./scripts/led_build -s "SYSTEM TOKEN NAME"
$ sudo ./scripts/led_install
```

#### Build Script Uninstall

If you have previously installed LED using build scripts, you can execute `led_uninstall.sh` to uninstall.
- Passing `-y` will answer yes to all prompts (does not remove data directories)
- Passing `-f` will remove data directories (be very careful with this)
- Passing in `-i` allows you to specify where your eosio installation is located

#### Supported Operating Systems

LEDGIS currently supports the following operating systems:  
1. Amazon Linux 2
2. CentOS 7
3. Ubuntu 18.04
4. MacOS 10.14 (Mojave)



#### LEDGIS features

1. Free Rate Limited Transactions
2. Low Latency Block confirmation (0.5 seconds)
3. Low-overhead Byzantine Fault Tolerant Finality
4. Designed for optional high-overhead, low-latency BFT finality
5. Smart contract platform powered by WebAssembly
6. Designed for Sparse Header Light Client Validation
7. Scheduled Recurring Transactions
8. Time Delay Security
9. Hierarchical Role Based Permissions
10. Support for Biometric Hardware Secured Keys (e.g. Apple Secure Enclave) 
11. Designed for Parallel Execution of Context Free Validation Logic
12. Designed for Inter Blockchain Communication

<!-- ## Resources

- [Website](https://kr.eoschrome.io/)

- [Telegram](https://t.me/eos_chrome)

- [Medium](https://medium.com/eoschrome)

- [Developer Portal](https://developers.eos.io) -->
