;
; BIND reverse data file for broadcast zone
;
$TTL	604800
@	IN	SOA	apple.com. root.localhost. (
			      1		; Serial
			 604800		; Refresh
			  86400		; Retry
			2419200		; Expire
			 604800 )	; Negative Cache TTL
;
@	IN	NS	apple.com.
@ 	IN	A	192.168.1.7
captive IN A 192.168.1.7
