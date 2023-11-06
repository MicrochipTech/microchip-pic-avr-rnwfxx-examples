#!/bin/sh
read -p "Enter the subCa folder(Default=subca):- " subca_folder
subca_folder=${subca_folder:-subca}

read -p "Enter the unique Device ID(Common Name):- " dev_id
dev_id=${dev_id:?Device ID missing}

country=US
state=AZ
location=Chandler
div=IoT
org="Custom Technology"


mkdir devcerts/${dev_id}

openssl genpkey -out devcerts/${dev_id}/${dev_id}.key -algorithm RSA -pkeyopt rsa_keygen_bits:2048

openssl req -new -key devcerts/${dev_id}/${dev_id}.key -out devcerts/${dev_id}/${dev_id}.csr -subj "//x=1/C=${country}/ST=${state}/L=${location}/O=${org}/OU=${div}/CN=${dev_id}"

openssl ca -config ${subca_folder}/subca.conf -in devcerts/${dev_id}/${dev_id}.csr -out devcerts/${dev_id}/${dev_id}.crt -extensions client_ext -batch -key 1234

openssl x509 -in devcerts/${dev_id}/${dev_id}.crt -out devcerts/${dev_id}/${dev_id}.pem



