# diary
A very basic diary application written in C++ and Qt. It keeps its data in an SQLite database. 
Encrypts the text only to keep it safe from your 9 year old sister.

Some people may not want a login screen and some may not want even the basic encryption.
I added an option to keep the diary unencrypted.

If you supply a password, program will use it to encrypt entries. So, if you forget your password you may say your diary goodbye.
Password protection works like this: First the password is hashed, using SHA512. Then resulting hash will be hashed with the entry key.
Entry key is automatically produced by SQLite during the entry creation. It is still a basic XOR operation.
