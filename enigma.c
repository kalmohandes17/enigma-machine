#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define NUM_ROTORS 8

/* Array of rotors */
const char* enigma_rotors[NUM_ROTORS+1] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ", //0
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", //1
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", //2
    "BDFHJLCPRTXVZNYEIWGAKMUSQO", //3
    "ESOVPZJAYQUIRHXLNFTGKDCMWB", //4
    "UKLBEPXWJVFZIYGAHCMTONDRQS", //5
    "JPGVOUMFYQBENHZRDKASXLICTW", //6
    "NZJHGRCXMYSWBOUFAIVLPEKQDT", //7
    "FKQHTLXOCBJSPDZRAMEWNIUYGV"  //8
};

/*
 * Convert a space-separated string of rotor indices into
 * an integer array of rotor indices
 *
 * @param rotor_ind_str   Space-separated string of rotor indices
 * @param num_rotors      Number of rotors provided in the string
 * @return                Integer array of rotor indices
 */
int* parse_rotor_indices(char* rotor_ind_str, int num_rotors) {
    // Allocate integer array
    int *rotor_indices = malloc(num_rotors * sizeof(int));
    
    // Populate array
    int index = 0;
    for (int i = 0; rotor_ind_str[i] != '\0'; ++i) {
        if (rotor_ind_str[i] >= '1' && rotor_ind_str[i] <= '8') {
            rotor_indices[index] = rotor_ind_str[i] - 48;
            index++;
        }   
    }    
    return rotor_indices;
}

/*
 * Create a 2D array of integers where
 * each row represents a rotor
 *
 * @param rotors          Integer array of rotor indices
 * @param num_rotors      Number of rotors provided
 * @return                2D array where each row represents a rotor
 */
int** set_up_rotors(int* rotors, int num_rotors) {
    // Allocate 2D Array
    int **rotor_config = malloc(num_rotors * sizeof(int*));

    for (int r = 0; r < num_rotors; ++r) {
        rotor_config[r] = malloc(ALPHABET_SIZE * sizeof(int));
    }

    // Populate 2D array
    for (int r = 0; r < num_rotors; ++r) {
        for (int c = 0; c < ALPHABET_SIZE; ++c) {
            rotor_config[r][c] = enigma_rotors[rotors[r]][c] - 65;
        }
    }
    return rotor_config;
}


/*
 * Rotate each rotor to the right by the
 * given number of rotations
 *
 * @param rotor_config   2D array of rotors
 * @param rotations      Number of rotations
 * @param num_rotors     Number of rotors provided
 */
void rotate_rotors(int** rotor_config, int rotations, int num_rotors) {
    rotations = rotations % ALPHABET_SIZE;

    for (int r = 0; r < num_rotors; ++r) {
        for (int i = 0; i < rotations; ++i) {
            
            int temp = rotor_config[r][ALPHABET_SIZE - 1];
            for (int c = ALPHABET_SIZE - 1; c >= 0; --c) {
                if (c == 0) {
                    rotor_config[r][c] = temp;
                }   
                else {
                    rotor_config[r][c] = rotor_config[r][c - 1];
                }
            }
        }
    }
}

/*
 * Encrypt the given message
 *
 * @param message        Message to encrypt
 * @param rotor_config   2D array of rotors
 * @param num_rotors     Number of rotors provided
 * @return               Encrypted message
 */
char* encrypt(char *message, int** rotor_config, int num_rotors) {    
    // Change all lowercase characters to uppercase
    for (int i = 0; message[i] != '\0'; ++i) {
        if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] -= 32;
        }
    }

    // Encrypt the message
    for (int r = 0; r < num_rotors; ++r) {
        for (int i = 0; message[i] != '\0'; ++i) {
            if (message[i] >= 'A' && message[i] <= 'Z') {
                int c = message[i] - 65;
                message[i] = (char)(rotor_config[r][c] + 65);
            }
        }
    }
    return message;
}

/*
 * Decrypt the given message
 *
 * @param message        Message to decrypt
 * @param rotor_config   2D array of rotors
 * @param num_rotors     Number of rotors provided
 * @return               Decrypted message
 */
char* decrypt(char *message, int** rotor_config, int num_rotors) {
    // Change all lowercase characters to uppercase
    for (int i = 0; message[i] != '\0'; ++i) {
        
        if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] -= 32;
        }
    }

    // Decrypt the message
    for (int r = 0; r < num_rotors; ++r) {
        for (int i = 0; message[i] != '\0'; ++i) {
            if (message[i] >= 'A' && message[i] <= 'Z') {
                int index;
                for (int c = 0; c < ALPHABET_SIZE; ++c) {
                    if (message[i] - 65  == rotor_config[r][c]) {
                        index = c;
                        break;
                    }
                }
                message[i] = (char)(index + 65);
            }
        }
    }
    return message;

}

/*
 * Format of command line input:
 * ./enigma e "JAVA" 3 "1 2 4" 0
 * 
 *    e    - mode (e for encrypt, d for decrypt)
 * "JAVA"  - message
 *    3    - number of rotors to use
 * "1 2 4" - indices of rotors to use
 *    0    - number of rotations of the rotors
 */
int main(int argc, char* argv[]) {
    // Assign command line arguments to variables
    char mode = *(argv[1]);
    char *message = argv[2];
    int num_rotors = atoi(argv[3]);
    int *rotor_indices = parse_rotor_indices(argv[4], num_rotors);
    int rotations = atoi(argv[5]);
    int **rotor_config = set_up_rotors(rotor_indices, num_rotors);

    rotate_rotors(rotor_config, rotations, num_rotors);

    // Message to be encrypted or decrypted depending on mode
    if (mode == 'e') {
        char *e_message = encrypt(message, rotor_config, num_rotors);
        printf("Encrypted message: %s\n", e_message);
    }
    else if(mode == 'd') {
        char *d_message = decrypt(message, rotor_config, num_rotors);
        printf("Decrypted message: %s\n", d_message);
    }

    // Free dynamically allocated memory
    for (int r = 0; r < num_rotors; ++r) {
        free(rotor_config[r]);
        rotor_config[r] = NULL;
    }
    free(rotor_indices);
    free(rotor_config);

    rotor_indices = NULL;
    rotor_config = NULL;
}


