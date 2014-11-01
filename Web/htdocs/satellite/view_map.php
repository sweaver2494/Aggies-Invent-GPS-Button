<?

include('dbinfo.php'); //Used for usernames and passwords
 
$dbcnx = mysql_connect ("$MyHostname", "$MyUsername", "$MyPassword");
mysql_select_db("$MyDatabaseName") or die(mysql_error());
?>
<html>
  <head>
	  <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
	  <title>Google Map with AIRS GPS Drop</title>

	  <link href="bootstrap/css/bootstrap.min.css" rel="stylesheet">
	  <link rel="stylesheet" type="text/css" href="view_map.css"/>

	  <div class="map">
	  <!-- THIS IS THE JAVASCRIPT CODE FOR THE MAP -->
		  <script src="http://maps.google.com/maps/api/js?v=3&sensor=false" type="text/javascript"></script>
			<script type="text/javascript">
				var icon = new google.maps.MarkerImage("http://maps.google.com/mapfiles/marker.png",
				new google.maps.Size(32, 32), 
				new google.maps.Point(0, 0),
				new google.maps.Point(16, 32));
				var center = null;
				var map = null;
				var currentPopup;
				var bounds = new google.maps.LatLngBounds();

				function addMarker(latitude, longitude, geo_event) {
					var pt = new google.maps.LatLng(latitude, longitude);
					bounds.extend(pt);
				 	var marker = new google.maps.Marker({
				 		position: pt,
				 		icon: icon,
				 		map: map
				 	});
				 	var popup = new google.maps.InfoWindow({
				 		content: geo_event,
				 		maxWidth: 300
				 	});
				 	google.maps.event.addListener(marker, "click", function() {
				 		if (currentPopup != null) {
				 			currentPopup.close();
				 			currentPopup = null;
				 		}
				 		popup.open(map, marker);
				 		currentPopup = popup;
				 	});
				 	google.maps.event.addListener(popup, "closeclick", function() {
				 		map.panTo(center);
				 		currentPopup = null;
				 	});
			 	}

				function initMap() {
				 	map = new google.maps.Map(document.getElementById("map"), {
				 		center: new google.maps.LatLng(30.6014, -96.3441),
				 		zoom: 13,
				 		mapTypeId: google.maps.MapTypeId.ROADMAP,
				 		mapTypeControl: false,
				 		mapTypeControlOptions: {
				 			style: google.maps.MapTypeControlStyle.HORIZONTAL_BAR
				 		},
				 		navigationControl: true,
				 		navigationControlOptions: {
				 			style: google.maps.NavigationControlStyle.SMALL
				 		}
			 		});

					<?
						$query = mysql_query("SELECT * FROM satellites"); //This is where the query is run! 
						while ($row = mysql_fetch_array($query)){
							$boardID=$row['boardID'];
							$latitude=$row['latitude'];
							$longitude=$row['longitude'];
							$event=$row['event'];
							echo ("addMarker($latitude, $longitude,'<b>$boardID</b><br/>$event');\n");
						}
				  ?>
					center = bounds.getCenter();
					map.fitBounds(bounds);
				}
			</script>
	  </div>
 </head>
 <body onload="initMap()" style="margin:0px; border:0px; padding:0px;">
 <div id="map"></div>
</html>
