all: demoCA demoCA/cacert.pem testuser_cert.pem server_cert.pem testuser_key.c testuser_cert.c ca.h

# Certification Authority

demoCA:
	mkdir demoCA
	mkdir demoCA/certs
	mkdir demoCA/crl
	mkdir demoCA/newcerts
	mkdir demoCA/private
	touch demoCA/index.txt
	echo 01 > demoCA/crlnumber

demoCA/private/cakey.pem: demoCA
	openssl genrsa -out demoCA/private/cakey.pem

demoCA/careq.pem: demoCA/private/cakey.pem
	openssl req -new -key demoCA/private/cakey.pem -out demoCA/careq.pem -subj '/CN=Certification Authority'

demoCA/cacert.pem: demoCA/careq.pem demoCA/private/cakey.pem openssl.cnf
	openssl ca -create_serial -out demoCA/cacert.pem -days 1095 -batch -keyfile demoCA/private/cakey.pem -selfsign -config openssl.cnf -extensions v3_ca -infiles demoCA/careq.pem

# Personal Certificate

testuser_key.pem:
	openssl genrsa -out testuser_key.pem 1024

testuser_req.pem: testuser_key.pem
	openssl req -new -nodes -key testuser_key.pem -out testuser_req.pem -days 365 -subj '/CN=testuser'

testuser_cert.pem: demoCA testuser_req.pem
	openssl ca -batch -policy policy_anything -out testuser_cert.pem -infiles testuser_req.pem

# Server Certificate

server_key.pem:
	openssl genrsa -out server_key.pem 1024

server_req.pem: server_key.pem
	openssl req -new -nodes -key server_key.pem -out server_req.pem -days 365 -subj '/CN=server'

server_cert.pem: demoCA server_req.pem
	openssl ca -batch -policy policy_anything -out server_cert.pem -infiles server_req.pem

# C version of personal certs

testuser_key.c: testuser_key.pem
	xxd -i testuser_key.pem > testuser_key.c
testuser_cert.c: testuser_cert.pem
	xxd -i testuser_cert.pem > testuser_cert.c

# C version of ca cert

ca.pem: demoCA/cacert.pem
	openssl x509 -in demoCA/cacert.pem -out ca.pem

ca.h: ca.pem
	xxd -i ca.pem > ca.h
#

clean:
	-rm testuser_req.pem server_req.pem

realclean:
	-rm testuser_key.pem testuser_key.c testuser_req.pem testuser_cert.pem testuser_cert.c server_key.pem server_req.pem server_cert.pem
