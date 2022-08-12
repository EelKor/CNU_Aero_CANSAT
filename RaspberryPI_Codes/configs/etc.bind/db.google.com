;
; BIND reverse data file for broadcast zone
;
$TTL	604800
@	IN	SOA	google.com. root.localhost. (
			      1		; Serial
			 604800		; Refresh
			  86400		; Retry
			2419200		; Expire
			 604800 )	; Negative Cache TTL
;
@	IN	NS	google.com.
@ 	IN	A	192.168.1.7
client3 IN 	A 	192.168.1.7
client1 IN 	A	192.168.1.7
