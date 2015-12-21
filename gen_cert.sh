openssl genrsa -out key.pem 1024
openssl req -new -key key.pem -x509 -subj '/CN=John Doe/emailAddress=jd@example.edu' -out cert.pem
# use xxd -i to generate c byte arrays
