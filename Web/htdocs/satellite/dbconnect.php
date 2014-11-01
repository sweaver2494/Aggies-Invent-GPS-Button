<?php
include('dbinfo.php'); //Used for usernames and passwords
$dbh = mysql_pconnect($MyHostname , $MyUsername, $MyPassword);
$selected = mysql_select_db($MyDatabaseName ,$dbh);
?>