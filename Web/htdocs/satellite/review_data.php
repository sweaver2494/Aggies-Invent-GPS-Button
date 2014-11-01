<?php 
    // Start MySQL Connection
    include('dbconnect.php'); 
?>

<html>
<head>
    <title>Arduino GPS Log</title>
    <style type="text/css">
        .table_titles, .table_cells_odd, .table_cells_even {
                padding-right: 20px;
                padding-left: 20px;
                color: #000;
        }
        .table_titles {
            color: #FFF;
            background-color: #666;
        }
        .table_cells_odd {
            background-color: #CCC;
        }
        .table_cells_even {
            background-color: #FAFAFA;
        }
        table {
            border: 2px solid #333;
        }
        body { font-family: "Trebuchet MS", Arial; }
    </style>
</head>

    <body>
        <h1>Arduino GPS Log</h1>
    <table border="0" cellspacing="0" cellpadding="4">
      <tr>
            <td class="table_titles">ID</td>
            <td class="table_titles">Board ID</td>
            <td class="table_titles">Date and Time</td>
            <td class="table_titles">Latitude</td>
            <td class="table_titles">Longitude</td>
          </tr>
<?php
    // Retrieve all records and display them
    $result = mysql_query("SELECT * FROM satellites ORDER BY id ASC");

    // Used for row color toggle
    $oddrow = true;

    // process every record
    while( $row = mysql_fetch_array($result) )
    {
        if ($oddrow) 
        { 
            $css_class=' class="table_cells_odd"'; 
        }
        else
        { 
            $css_class=' class="table_cells_even"'; 
        }

        $oddrow = !$oddrow;

        echo '<tr>';
        echo '   <td'.$css_class.'>'.$row["id"].'</td>';
        echo '   <td'.$css_class.'>'.$row["boardID"].'</td>';
        echo '   <td'.$css_class.'>'.$row["event"].'</td>';
        echo '   <td'.$css_class.'>'.$row["longitude"].'</td>';
        echo '   <td'.$css_class.'>'.$row["latitude"].'</td>';
        echo '</tr>';
    }
?>
    </table>
    </body>
</html>