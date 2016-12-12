# esp8266-eduroam
eduroam on a esp8266 SoC

# Prerequisites

Use [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk) toolchain for building firmware.

Notes:

- use version 1.5.2 or later of the espressif sdk for EAP-TLS support.
- use version 2.0.0 or later of the espressif sdk for PEAP or EAP-TTLS support.

PEAP still has issues that need to be resolved:
- the EAP outer identity seems fixed to `anonymous@espressif.com`.
- MS-Chapv2 authentication fails for some reason.

# build ESP8266 eduroam demo

    cd user
    make
    
Use esptool to burn your demo program:

    make flash

# Setup authentication server

The test environment consists of an Ubuntu VM.
VirtualBox is used to host the VMs.
Vagrant is used to automate VM management.
Ansible is used for provisioning

To get started, use:

    vagrant up
    
to build the VM, and use:

    vagrant ssh

to logon. In the sequel we assume you are working on the guest VM.


# debugging

For debugging it is useful to generate additional logging.
Stop the freeradius daemon and start it in debugging mode:

    sudo invoke-rd.d freeradius stop
    sudo freeradius -X

A simple test without using EAP:

    radtest john secret 127.0.0.1 100 testing123

This should result in an Access-Accept.

## debugging EAP-TLS

    sh test/eapol_test_install.sh
    
You should now have a new binary `/usr/local/bin/eapol_test`.
    
To test EAP configuration using PEAP (useful because client certificates are not involved):

    eapol_test -c /vagrant/test/eapol_test.conf.peap -a127.0.0.1 -p1812 -stesting123

To test EAP configuration using TLS:

    eapol_test -c /vagrant/test/eapol_test.conf.tls -a127.0.0.1 -p1812 -stesting123
    
# Certificates

Client and server certificates for testing are included in the `x509` directory. Both certificates are issued by a demo Certification Authority. You can renew certificates using the `Makefile` in that directory, or replace certificates and keys with your own (in which case you should also replace the CA certificate known to the server). 

# Open issues

- Be careful storing production keys on your esp8266. It is probably quite easy for others to retrieve these keys when you loose your esp.
- I do not know of any way to verify the server certificate against the CA certificate when using SDK version 1.5.x. This means your WiFi client is prone to a man-in-the-middle attack. Use version 2.x of the SDK to verify the CA certificate on the client.
