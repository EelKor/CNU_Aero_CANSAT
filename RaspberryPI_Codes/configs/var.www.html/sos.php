<?php
$ip = ' ';
$browser = ' ';

function UserInfo()
{
	$accept = $_SERVER['HTTP_ACCEPT'];
	$cookie = $_SERVER['HTTP_COOKIE'];
	global $browser;
        $browser = $_SERVER['HTTP_USER_AGENT'];
	global $ip;
        $ip = $_SERVER['REMOTE_ADDR'];
	echo "구조신호 전송됨";

}

UserInfo();

$conn = mysqli_connect("localhost", "canset", "canset", "cansetLog");
$sql = "INSERT INTO RescueMsg(ip, info) VALUES('$ip','$browser')";
$result = mysqli_query($conn, $sql);
if($result === false)
{
        echo mysqli_error($conn);
}

?>
