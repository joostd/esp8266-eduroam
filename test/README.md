# test setup

Test EAP-MD5, to see if EAP is configured properly:

	./eap-md5

Install `eapol_test` utility, to be able to test certificate config:

	sudo sh eapol_test_install.sh 

Test PEAP, to check if server certicates are ok:

	eapol_test -c eapol_test.conf.peap -a127.0.0.1 -p1812 -stesting123

Test EAP-TLS, to check if user certificates are ok:

	eapol_test -c eapol_test.conf.tls -a127.0.0.1 -p1812 -stesting123
