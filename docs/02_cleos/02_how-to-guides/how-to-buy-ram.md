
## Overview
This guide provides instructions on how to buy RAM for an EOSIO blockchain account using the cleos CLI tool. RAM is a system resource used to store blockchain state such as smart contract data and account information.

The example uses `cleos` to buy RAM for the alice account. The alice account pays for the RAM and the alice@active permisssion authorizes the transaction.  

## Before you Begin
Make sure you meet the following requirements: 

* Install the currently supported version of `cleos.`
[[info | Note]]
| `Cleos` is bundled with the LEDGIS software. [Installing LEDGIS](../../00_install/index.md) will install the `cleos` and `keosd` command line tools. 
* You have access to an LEDGIS blockchain and the `led.system` reference contract from [`led.public.contracts`](https://github.com/ibct-dev/led.public.contracts) repository is deployed and used to manage system resources.
* You have an LEDGIS account and access to the account's private key.
* You have sufficient [tokens allocated](how-to-transfer-an-led.token-token.md) to your account.

## Reference
See the following reference guides for command line usage and related options:

* [cleos system buyram](../03_command-reference/system/system-buyram.md) command

## Buy Ram Procedure

The following step shows you how to buy RAM

1. Run the following command to buy RAM worth 0.1 of `SYS` tokens for the alice account:

```shell
cleos system buyram alice alice "0.1 SYS" -p alice@active
```

**Where**
* `alice` = payer, the account paying for RAM.
* `alice` = receiver, the account receiving bought RAM.
* `0.1 SYS` = The amount of tokens to pay for RAM.
* `-p alice@active` = The permission used to authorize the payment, in this case the active permission.  

**Example Output**

```sh
executed transaction: aa243c30571a5ecc8458cb971fa366e763682d89b636fe9dbe7d28327d1cc4e9  128 bytes  283 us
#         eosio <= eosio::buyram                {"payer":"alice","receiver":"alice","quant":"0.1000 SYS"}
#   led.token <= led.token::transfer        {"from":"alice","to":"led.ram","quantity":"0.0995 SYS","memo":"buy ram"}
#   led.token <= led.token::transfer        {"from":"alice","to":"led.ramfee","quantity":"0.0005 SYS","memo":"ram fee"}
#         alice <= led.token::transfer        {"from":"alice","to":"led.ram","quantity":"0.0995 SYS","memo":"buy ram"}
#     led.ram <= led.token::transfer        {"from":"alice","to":"led.ram","quantity":"0.0995 SYS","memo":"buy ram"}
#         alice <= led.token::transfer        {"from":"alice","to":"led.ramfee","quantity":"0.0005 SYS","memo":"ram fee"}
#  led.ramfee <= led.token::transfer        {"from":"alice","to":"led.ramfee","quantity":"0.0005 SYS","memo":"ram fee"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
## Summary
In conclusion, by following these instructions you are able to purchase RAM, with a specified amount of tokens, for the specified accounts.