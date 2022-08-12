<?php
$conn = mysqli_connect("localhost", "canset", "canset", "cansetLog");
$sql = "INSERT INTO RescueMsg(ip, info) VALUES('test1','test2')";
$result = mysqli_query($conn, $sql);
if($result === false)
{
	echo mysqli_error($conn);
}

?>
