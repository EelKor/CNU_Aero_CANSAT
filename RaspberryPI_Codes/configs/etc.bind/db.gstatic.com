;
; BIND reverse data file for broadcast zone
;
$TTL	604800
@	IN	SOA	gstatic.com. root.localhost. (
			      1		; Serial
			 604800		; Refresh
			  86400		; Retry
			2419200		; Expire
			 604800 )	; Negative Cache TTL
;
@	IN	NS	gstatic.com.
@ 	IN	A	192.168.1.7
connectivitycheck IN A 192.168.1.7
