#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#define CLIENT_ID "6e73370bdfd84ccba9c2da88d32d1211"
#define CLIENT_SECRET "64066a67b6704a478d69bfa1087609f1"
#define REDIRECT_URI "http://127.0.0.1:8888/callback"

// Base64 encoding function
char *base64_encode(const char *data, size_t input_length) {
    BIO *bio, *b64;
    BUF_MEM *buffer_ptr;
    char *encoded_data;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, data, input_length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buffer_ptr);
    encoded_data = (char *)malloc(buffer_ptr->length);
    memcpy(encoded_data, buffer_ptr->data, buffer_ptr->length - 1);
    encoded_data[buffer_ptr->length - 1] = '\0';
    BIO_free_all(bio);
    return encoded_data;
}

void get_access_token(const char *code) {
    CURL *curl;
    CURLcode res;
    char post_data[512];

    // Prepare POST data
    snprintf(post_data, sizeof(post_data),
             "grant_type=authorization_code&code=%s&redirect_uri=%s",
             code, REDIRECT_URI);

    // Base64 encode the client ID and client secret
    char client_credentials[256];
    snprintf(client_credentials, sizeof(client_credentials), "%s:%s", CLIENT_ID, CLIENT_SECRET);
    char *encoded_credentials = base64_encode(client_credentials, strlen(client_credentials));

    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = NULL;
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Basic %s", encoded_credentials);
        
        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Access Token: Success!\n");
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    free(encoded_credentials);
    curl_global_cleanup();
}

int main() {
    const char *auth_code = "AQBU2LOXceVCBowJVU_6j78tjFAKzXaOflGm1FLQstBdKUOvh8bZbTUfPZ6XOFUBVRG0VwD6YOwTmbh9v4jGc1V7ce3S0CKNB5Ho0GwacXFJJGdWul7RTrSNtMGJKXKvOQcf6pCpnooxBYCjmzxjD3JOl6Wf7gmW1fguRygHUF0JoOF-c2xJ3MHlm0ILGISvls8cHKet_ybHjn3dv7Ew_cUgKLegw6nw7n-lGpKWbeJ0XhhY189MqA"; // Replace with the actual code you get
    get_access_token(auth_code);
    return 0;
}

