#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int message_length;
char *message = NULL;
char *enc_basis = NULL;
char *dec_basis = NULL;
int eve_bool;


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

// Helper function to simulate Eve intercepting the entire message sent by Alice to Bobs
char *intercept(char *enc_str){
    int length = strlen(enc_str);

    char *dec_basis = generateRandomBasis(length);
    printf("Eve's randomly generated decryption basis: %s\n", dec_basis);
    char *dec_str = decrypt(enc_str, dec_basis);
    printf("Eve has decrypted Alice's encrypted string to: %s\n", dec_str);
    char *intercepted_str = encrypt(dec_str, dec_basis);
    printf("Eve will now encrypt the (decrypted) string back with the encryption basis to generate: %s\n", intercepted_str);
    printf("Eve will forward this encrypted (intercepted) string onto Bob\n");
    printf("\n");
    return intercepted_str;
}

// Helper function to produce print the shared information between the basis and the bits associated with the matching indices
void printSimStats(char *enc_basis, char *dec_basis, char *dec_str){
    
    int key_exists = 0;

    int num_shared_basis = 0;
    int num_shared_basis_bits = 0;
    int num_check_mismatches = 0;

    printf("STEP 7: The indexes for the matching basis are: ");
    int size = strlen(enc_basis);
    for (int i = 0; i < size; i++){
        if (enc_basis[i] == dec_basis[i]){
            key_exists = 1;
            printf("%d ", i);
            num_shared_basis++;
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
        printf("In this case, there is no shared information as no indexes match.\n");
    }

    for (int i = 0; i < size; i++){
        if (enc_basis[i] == dec_basis[i]){
            printf("%c", message[i]);
            if (message[i] == dec_str[i]){
                num_shared_basis_bits++;
            }
        }
    }

    printf("\n\n");

    printf("How many bits would you like Bob to release into public so Alice can check for the presence of Eve?\n");
    printf("Bob may MAX up to the number of bits with matching basis (%d): \n", num_shared_basis);
    int num_bits_to_check;
    scanf("%d", &num_bits_to_check);
    if (num_bits_to_check > num_shared_basis){
        num_bits_to_check = num_shared_basis;
    }
    printf("STEP 8: Releasing %d bits to check for the presence of Eve.\n", num_bits_to_check);

    int bits_checked = 0;

    for (int i = 0; i < size; i++){
        if ((enc_basis[i] == dec_basis[i]) && (bits_checked < num_bits_to_check)){
            if (!(message[i] == dec_str[i])){
                num_check_mismatches++;
            }
            bits_checked++;
        }
    }

    if (num_check_mismatches > 0){
        printf("STEP 9: %d mismatches found from %d bits! Presence of Eve detected!\n", num_check_mismatches, num_bits_to_check);
    } else {
        printf("STEP 9: No mismatches found. No presence of Eve detected\n");
    }

    printf("\n");
    printf("SIMULATION STATS:\n");
    printf("Total string length Alice sends to Bob: %d\n", message_length);
    printf("Total number of matching basis among Alice and Bob: %d\n", num_shared_basis);
    printf("Number of bits Bob released to check for the presence of Eve: %d\n", num_bits_to_check);
    printf("Number of mismatches found by Alice from released bits: %d\n", num_check_mismatches);
    float percent_error = fabs((((float)(num_bits_to_check - num_check_mismatches) - (float)num_bits_to_check) / (float)num_bits_to_check) * 100);
    printf("Percent error detected by Alice and Bob from mismatched bits to released bits (to two decimal places): %.2f%%\n", percent_error);

    printf("\n");
    percent_error = fabs((((float)num_shared_basis_bits - (float)num_shared_basis) / (float)num_shared_basis) * 100);
    printf("(OVERALL) Total number of mismatching bits from matching basis: %d\n", num_shared_basis - num_shared_basis_bits);
    printf("(OVERALL) Percent error from all shared bits to shared basis (to two decimal places): %.2f%%\n", percent_error);

    if (eve_bool == 0){
        printf("\n");
        printf("Note: Percent error is guaranteed to be 0.00%% because Eve is not present. Try running the simulation again with Eve intercepting to observe that Alice and Bob will have fewer remaining shared bits among their shared basis!\n");
    }

    printf("\n");
}

int main(int argc, char *argv[]){
    srand(clock());
    // Usage and sanity check for running the program
    if (!((argc == 1) ||(argc == 2) || (argc == 4) || (argc == 6))){
        printf("Usage: %s <string in binary> -e <encryption basis> -d <decription basis>\n", argv[0]);
        printf("To use random encryption basis, leave \"-e <encryption basis>\" out\n");
        printf("To use random decription basis, leave \"-d <decription basis>\" out\n");
        printf("Input the encryption and decription basis as a string of \"r\" and \"d\" where \"r\" is chosen rectilinear basis and \"d\" is a chosen diagonal basis for a bit\n");
        printf("Chosen basis must have the same string length as Alice's string\n");
        return 1;
    }
    // Use user inputted parameters
    if (argc != 1){
        for (int i = 1; i < argc; i++){
            if (i == 1){
                message = argv[i];
                message_length = strlen(message);
                printf("STEP 1: Chosen Alice's string: %s\n", message);
                if (isBinary(message) != 1){
                    printf("Chosen string is not a binary string. Exiting\n");
                    return 0;
                }
                printf("\n");
                printf("Rectilinear Basis is represented by \"r\"\n");
                printf("Diagonal Basis is represented by \"d\"\n");
            }
            // Use user inputted encryption basis if provided
            if (strcmp(argv[i], "-e") == 0){
                enc_basis = argv[i + 1];
                printf("STEP 2: Alice's chosen encryption basis: %s\n", enc_basis);
                if (message_length != strlen(enc_basis)){
                    printf("Chosen encryption basis is not of same length as Alice's string. Exiting\n");
                    return 0;
                }
                if (isBasis(enc_basis) != 1){
                    printf("Chosen encoding basis is not a valid basis (must only contain characters of \"r\" and \"d\"). Exiting\n");
                    return 0;
                }
            // Use user inputted decryption basis if provided
            } else if (strcmp(argv[i], "-d") == 0){
                dec_basis = argv[i + 1];
                if (message_length != strlen(dec_basis)){
                    printf("Chosen decryption basis: %s\n", dec_basis);
                    printf("Chosen decryption basis is not of same length as Alice's string. Exiting\n");
                    return 0;
                }
                if (isBasis(dec_basis) != 1){
                    printf("Chosen decryption basis: %s\n", dec_basis);
                    printf("Chosen decryption basis is not a valid basis (must only contain characters of \"r\" and \"d\"). Exiting\n");
                    return 0;
                }
            }
        }
    // Use random parameters if no arguments were given
    } else if (argc == 1){
        printf("Enter desired length of string for Alice to send Bob: ");
        scanf("%d", &message_length);
        message = generateRandomMessage(message_length);
        printf("STEP 1: Randomly chosen Alice's string of length %d: %s\n", message_length, message);
        printf("Note: You may also specify protocol details by using the format:\n");
        printf("    Usage: %s <string in binary> -e <encryption basis> -d <decription basis>\n", argv[0]);
        if (isBinary(message) != 1){
            printf("Chosen string is not a binary string. Exiting\n");
            return 0;
        }
        printf("\n");
        printf("Rectilinear Basis is represented by \"r\"\n");
        printf("Diagonal Basis is represented by \"d\"\n");
    }
    if (enc_basis == NULL){
        enc_basis = generateRandomBasis(message_length);
        printf("STEP 2: Alice's random encryption basis: %s\n", enc_basis);
    }

    printf("\n");

    // Prompt user to determine if they want Eve to intercept the string Alice sends Bob
    printf("Would you like the simulation to have Eve intercept the string Alice sends Bob? [y/n]: ");

    char eve_input;
    scanf(" %c", &eve_input);

    eve_bool = 0;

    if (eve_input == 'y' || eve_input == 'Y') {
        eve_bool = 1;
        printf("Eve will intercept!\n");
    } else {
        printf("Eve will NOT intercept\n");
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

    char *enc_str;
    char *dec_str;

    enc_str = encrypt(message, enc_basis);
    printf("STEP 3: Alice's encoded message is: %s\n", enc_str);

    if (dec_basis == NULL){
        dec_basis = generateRandomBasis(message_length);
        printf("STEP 4: Bob's random decryption basis: %s\n", dec_basis);
    } else {
        printf("STEP 4: Bob's chosen decryption basis: %s\n", dec_basis);
    }

    printf("STEP 5: Bob will now decrypt the encoded string with the decryption key to get: %s\n", dec_str);
    printf("\n");

    if (eve_bool == 1){
        char * int_str = intercept(enc_str);
        dec_str = decrypt(int_str, dec_basis);
    } else {
        dec_str = decrypt(enc_str, dec_basis);
    }


    printf("STEP 6: Bob will now broadcast the basis used by him, and Alice will confirm the bits that have matching basis.\n");
    printSimStats(enc_basis, dec_basis, dec_str);
    return 0;
}
