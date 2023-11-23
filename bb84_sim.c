#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int message_length;
char *message = NULL;
char *enc_basis = NULL;
char *dec_basis = NULL;


// Helper function to generate a random basis
char *generateRandomBasis(int size){
    int randNum;
    char *randStr = malloc((size+1)*sizeof(char));

    for (int i = 0; i < size; i++){
        randNum = rand()%2;
        if (randNum == 0){
            randStr[i] = 'r';
        } else {
            randStr[i] = 'd';
        }
    }
    randStr[size] = '\0';
    return randStr;
}

// Function to generate a random string
char *generateRandomMessage(int size){
    int randNum;
    char *randStr = malloc((size+1)*sizeof(char));

    for (int i = 0; i < size; i++){
        randNum = rand() % 2;
        if (randNum == 0){
            randStr[i] = '0';
        } else {
            randStr[i] = '1';
        }
    }
    randStr[size] = '\0';
    return randStr;
}

// Helper function to check if a string is in binary
int isBinary(const char *str){
    while (*str){
        if ((*str != '0') && (*str != '1')){
            // str is not binary
            return 0;
        }
        str++;
    }
    // str is binary
    return 1;
}

// Helper function to check if a string is a valid basis
int isBasis(const char *str){
    while (*str){
        if ((*str != 'r') && (*str != 'd')){
            // str is not a basis
            return 0;
        }
        str++;
    }
    // str is a basis
    return 1;
}

// Helper function to generate a random bit of 1 or 0
char generateRandomBit(){
    int randBit = rand() % 2;
    if (randBit == 0){
        return '0';
    } else {
        return '1';
    }
}

// Helper function to encrypt the message with the encryption basis
char *encrypt(char *message, char *enc_basis){
    int length = strlen(message);
    char *enc_str = malloc((length + 1) * sizeof(char));

    for (int i = 0; message[i] != '\0'; i++){
        if (message[i] == '0'){
            if (enc_basis[i] == 'r'){
                enc_str[i] = 'H';
            } else if (enc_basis[i] == 'd'){
                enc_str[i] = 'R';
            }
        } else if (message[i] == '1'){
            if (enc_basis[i] == 'r'){
                enc_str[i] = 'V';
            } else if (enc_basis[i] == 'd'){
                enc_str[i] = 'L';
            }
        }
    }
    return enc_str;
}


// Helper function to decrypt the encrypted string with the decryption basis
char *decrypt(char *enc_str, char *dec_basis){
    int length = strlen(enc_str);

    char *dec_str = malloc((length + 1) * sizeof(char));
    char temp;

    for (int i = 0; enc_str[i] != '\0'; i++){
        temp = generateRandomBit();
        if (enc_str[i] == 'V'){
            if (dec_basis[i] == 'r'){
                dec_str[i] = '1';
            } else if (dec_basis[i] == 'd'){
                dec_str[i] = temp;
            }
        } else if (enc_str[i] == 'H'){
            if (dec_basis[i] == 'r'){
                dec_str[i] = '0';
            } else if (dec_basis[i] == 'd'){
                dec_str[i] = temp;
            }
        } else if (enc_str[i] == 'R'){
            if (dec_basis[i] == 'r'){
                dec_str[i] = temp;
            } else if (dec_basis[i] == 'd'){
                dec_str[i] = '0';
            }
        } else if (enc_str[i] == 'L'){
            if (dec_basis[i] == 'r'){
                dec_str[i] = temp;
            } else if (dec_basis[i] == 'd'){
                dec_str[i] = '1';
            }
        }
    }
    return dec_str;
}

// Helper function to produce find the shared information between the basis and the bits associated with the matching indices
void findSharedInformation(char *enc_basis, char *dec_basis){
    int key_exists = 0;
    printf("The indexes for the matching basis are: ");
    int size = strlen(enc_basis);
    for (int i = 0; i < size; i++){
        if (enc_basis[i] == dec_basis[i]){
            key_exists = 1;
            printf("%d ", i);
        }
    }
    if (key_exists == 0){
        printf("NONE. There are no matching basis");
    }
    printf("\n");
    printf("The rest of indexes (without matching basis) are discarded for the resulting shared information\n");
    printf("\n");
    printf("Thus, the shared information of Alice and Bob is the binary string constructed from the matching indexes: ");

    if (key_exists == 0){
        printf("In this case, there is no shared information as no indexes match.");
    }
    for (int i = 0; i < size; i++){
        if (enc_basis[i] == dec_basis[i]){
            printf("%c", message[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    srand(clock());
    // Usage and sanity check for running the program
    if (!((argc == 1) ||(argc == 2) || (argc == 4) || (argc == 6))){
        printf("Usage: %s <message in binary> -e <encryption basis> -d <decription basis>\n", argv[0]);
        printf("To use random encryption basis, leave \"-e <encryption basis>\" out\n");
        printf("To use random decription basis, leave \"-d <decription basis>\" out\n");
        printf("Input the encryption and decription basis as a string of \"r\" and \"d\" where \"r\" is chosen rectilinear basis and \"d\" is a chosen diagonal basis for a bit\n");
        printf("Chosen basis must have the same string length as the message\n");
        return 1;
    }
    // Use user inputted parameters
    if (argc != 1){
        for (int i = 1; i < argc; i++){
            if (i == 1){
                message = argv[i];
                message_length = strlen(message);
                printf("Chosen Alice's message: %s\n", message);
                if (isBinary(message) != 1){
                    printf("Chosen message is not a binary string. Exiting\n");
                    return 0;
                }
                printf("\n");
                printf("Rectilinear Basis is represented by \"r\"\n");
                printf("Diagonal Basis is represented by \"d\"\n");
            }
            // Use user inputted encryption basis if provided
            if (strcmp(argv[i], "-e") == 0){
                enc_basis = argv[i + 1];
                printf("Chosen encryption basis: %s\n", enc_basis);
                if (message_length != strlen(enc_basis)){
                    printf("Chosen encryption basis is not of same length as message. Exiting\n");
                    return 0;
                }
                if (isBasis(enc_basis) != 1){
                    printf("Chosen encoding basis is not a valid basis (must only contain characters of \"r\" and \"d\"). Exiting\n");
                    return 0;
                }
            // Use user inputted decryption basis if provided
            } else if (strcmp(argv[i], "-d") == 0){
                dec_basis = argv[i + 1];
                printf("Chosen decryption basis: %s\n", dec_basis);
                if (message_length != strlen(dec_basis)){
                    printf("Chosen decryption basis is not of same length as message. Exiting\n");
                    return 0;
                }
                if (isBasis(dec_basis) != 1){
                    printf("Chosen decryption basis is not a valid basis (must only contain characters of \"r\" and \"d\"). Exiting\n");
                    return 0;
                }
            }
        }
    // Use random parameters if no arguments were given
    } else if (argc == 1){
        // Generate a random number from 10 to 100 to set message length
        message_length = rand()%91;
        message_length += 10;
        message = generateRandomMessage(message_length);
        printf("Randomly chosen Alice's message of length %d: %s\n", message_length, message);
        printf("Note: You may also specify protocol details by using the format:\n");
        printf("    Usage: %s <message in binary> -e <encryption basis> -d <decription basis>\n", argv[0]);
        if (isBinary(message) != 1){
            printf("Chosen message is not a binary string. Exiting\n");
            return 0;
        }
        printf("\n");
        printf("Rectilinear Basis is represented by \"r\"\n");
        printf("Diagonal Basis is represented by \"d\"\n");
    }
    if (enc_basis == NULL){
        enc_basis = generateRandomBasis(message_length);
        printf("Random encryption basis: %s\n", enc_basis);
    }
    if (dec_basis == NULL){
        dec_basis = generateRandomBasis(message_length);
        printf("Random decryption basis: %s\n", dec_basis);
    }

    printf("\n");

    printf("Using the following bit encoding:\n");
    printf("    Rectilinear Basis (r):\n");
    printf("        0 is encoded to H (Horizontal), ↔ (0°)\n");
    printf("        1 is encoded to V (Vertical), ↕ (90°)\n");
    printf("    Diagonal Basis (d):\n");
    printf("        0 is encoded to R (45° Right), ⤢ (45°)\n");
    printf("        1 is encoded to L (-45° Left), ⤡ (-45°)\n");

    printf("\n");

    char *enc_str = encrypt(message, enc_basis);
    printf("Alice's encoded message is: %s\n", enc_str);
    char *dec_str = decrypt(enc_str, dec_basis);
    printf("Bob will now decrypt the encoded message with the decryption key to get: %s\n", dec_str);
    printf("\n");
    printf("Bob will now broadcast the basis used by him, and Alice will confirm the bits that have matching basis.\n");
    findSharedInformation(enc_basis, dec_basis);
    return 0;
}
