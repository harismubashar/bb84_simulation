#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

size_t message_length;
char *message = NULL;
char *enc_basis = NULL;
char *dec_basis = NULL;


// Function to generate a random string
char *generateRandomBasis(int size){
    int randomNumber;
    // Allocate memory for the string (including space for the null terminator)
    char *randomString = malloc((size + 1) * sizeof(char));

    // Check if memory allocation is successful
    if (randomString == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Generate the random string
    for (int i = 0; i < size; i++) {
        // Randomly choose "r" or "d" with equal probability
        randomNumber = rand() % 2;
        
        if (randomNumber == 0) {
            randomString[i] = 'r';
        } else {
            randomString[i] = 'd';
        }
    }

    // Add the null terminator at the end
    randomString[size] = '\0';

    return randomString;
}

int isBinaryString(const char *str){
    while (*str) {
        if (*str != '0' && *str != '1') {
            return 0; // Not a binary string
        }
        str++;
    }
    return 1; // Binary string
}

char generateRandomBit() {
    // Generate a random number (0 or 1)
    int randomBit = rand() % 2;

    if (randomBit == 0){
        return '0';
    } else {
        return '1';
    }
}

char *encrypt(char *message, char *enc_basis){
    // Get the length of other string
    size_t length = strlen(message);

    // Allocate memory for somestr with the same length as other string
    char *enc_str = malloc((length + 1) * sizeof(char));

    // Check if memory allocation is successful
    if (enc_str == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

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


char *decrypt(char *enc_str, char *dec_basis){
    // Get the length of other string
    size_t length = strlen(enc_str);

    // Allocate memory for somestr with the same length as other string
    char *dec_str = malloc((length + 1) * sizeof(char));

    char temp;

    // Check if memory allocation is successful
    if (dec_str == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

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

int isBasisString(const char *str) {
    while (*str) {
        if (*str != 'r' && *str != 'd') {
            return 0; // Not a basis string
        }
        str++;
    }
    return 1; // basis string
}

void findSharedInformation(char *enc_basis, char *dec_basis){
    int key_exists = 0;
    printf("The indexes for the matching basis are: ");
    int size = strlen(enc_basis);
    for (int i = 0; i < size; i++) {
        if (enc_basis[i] == dec_basis[i]) {
            key_exists = 1;
            printf("%d ", i);
        }
    }
    if (key_exists == 0){
        printf(" NONE. There are no matching basis");
    }
    printf("\n");
    printf("The rest of indexes (without matching basis) are discarded for the resulting shared information\n");
    printf("\n");
    printf("Thus, the shared information of Alice and Bob is the binary string constructed from the matching indexes: ");

    if (key_exists == 0){
        printf("In this case, there is no shared information as no indexes match.");
    }
    for (int i = 0; i < size; i++) {
        if (enc_basis[i] == dec_basis[i]) {
            printf("%c", message[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    srand(clock());
    // Check if the correct number of arguments are provided
    if (!((argc == 2) || (argc == 4) || (argc == 6))){
        printf("Usage: %s <message in binary> -e <encryption basis> -d <decription basis>\n", argv[0]);
        printf("To use random encryption basis, leave \"-e <encryption basis>\" out\n");
        printf("To use random decription basis, leave \"-d <decription basis>\" out\n");
        printf("Input the encryption and decription basis as a string of \"r\" and \"d\" where \"r\" is chosen rectilinear basis and \"d\" is a chosen diagonal basis for a bit\n");
        printf("Chosen basis must have the same string length as the message\n");
        return 1; // Exit with an error code
    }

    for (int i = 1; i < argc; i++){
        if (i == 1){
            message = argv[i];
            message_length = strlen(message);
            printf("Chosen Alice's message: %s\n", message);
            if (isBinaryString(message) != 1){
                printf("Chosen message is not a binary string. Exiting\n");
                return 0;
            }
            printf("\n");
            printf("Rectilinear Basis is represented by \"r\"\n");
            printf("Diagonal Basis is represented by \"d\"\n");
        }
        if (strcmp(argv[i], "-e") == 0){
            // Handle encryption string
            enc_basis = argv[i + 1];
            printf("Chosen encryption basis: %s\n", enc_basis);
            if (message_length != strlen(enc_basis)){
                printf("Chosen encryption basis is not of same length as message. Exiting\n");
                return 0;
            }
            if (isBasisString(enc_basis) != 1){
                printf("Chosen encoding basis is not a valid basis (must only contain characters of \"r\" and \"d\"). Exiting\n");
                return 0;
            }
        } else if (strcmp(argv[i], "-d") == 0){
            // Handle decryption string
            dec_basis = argv[i + 1];
            printf("Chosen decryption basis: %s\n", dec_basis);
            if (message_length != strlen(dec_basis)){
                printf("Chosen decryption basis is not of same length as message. Exiting\n");
                return 0;
            }
            if (isBasisString(dec_basis) != 1){
                printf("Chosen decryption basis is not a valid basis (must only contain characters of \"r\" and \"d\"). Exiting\n");
                return 0;
            }
        }
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
