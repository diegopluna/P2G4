#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cgic.h>

void makeFormRequest(const char* url, const char* data) {
    printf("Making form request to URL: %s\n", url);
    printf("Form Data: %s\n", data);
}

int cgiMain() {
    cgiHeaderContentType("text/html");  

    char name[256];
    char email[256];
    cgiFormString("name", name, sizeof(name));
    cgiFormString("email", email, sizeof(email));

    char data[512];
    snprintf(data, sizeof(data), "name=%s&email=%s", name, email);

    const char* url = "https://example.com/submit-form";
    makeFormRequest(url, data);

    printf("<html><body>");
    printf("<h1>Form Submitted</h1>");
    printf("<p>Thank you, %s, for submitting the form. We have received your information.</p>", name);
    printf("</body></html>");

    return 0;
}
