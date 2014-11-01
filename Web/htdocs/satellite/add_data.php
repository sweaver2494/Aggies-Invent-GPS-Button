<?php
		include('dbconnect.php'); 


    // Prepare the SQL statement
    $SQL = "INSERT INTO arduino.satellites (boardID, latitude ,longitude) VALUES ('".$_GET["boardID"]."','".$_GET["latitude"]."','".$_GET["longitude"]."')";     

    // Execute SQL statement
    mysql_query($SQL);

    // Go to the review_data.php (optional)
    //header("Location: review_data.php");
?>