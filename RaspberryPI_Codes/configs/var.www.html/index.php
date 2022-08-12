<html>
	<head>
		<meta charset="utf-8" name="viewport" content="user-scalable=yes, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0,width=device-width"/>

		<title>	WE CAN Help </title>
	</head>

	<body>
	<h1> 2022 캔위성 경진대회 </h1>
	<h1> WeCANHelp 팀 </h1>

	<?php
		$conn = mysqli_connect("localhost","canset","canset","cansetLog");
		$sql = "SELECT * FROM notice";
		$result = mysqli_query($conn, $sql);
		
		if(mysqli_num_rows($result) > 0)
		{
			$table = '
				<table border=1>
				<tr>
					<th> 번호 </th>
					<th> 내용 </th>
					<th> 발령권자 </th>
					<th> 발령일시 </th>
				</tr>
				';
			while($row = mysqli_fetch_array($result))
			{
				$table .= '
					<tr>
						<td>'.$row["no"].'</td>
						<td>'.$row["content"].'</td>
						<td>'.$row["whos"].'</td>
						<td>'.$row["date"].'</td>
					</tr>
					';
			}
			$table .= '</table>';
			echo $table;
		}	
	?>
		<h3> 비상통신망에 접속하려면 접속 버튼을 누르십시오 </h2>
		<h3> To Connect Emergency Network, Press Connect Button </h2>
		<button type='button' class='navyBtn' onClick="location.href='home.html'">
		<input type='button' value = "생존자 네트워크 접속">
		
		<button type='button' class='navyBtn' onClick="location.href='sos.php'">
		<input type='button' value = "구조요청">

	</body>
</html>
