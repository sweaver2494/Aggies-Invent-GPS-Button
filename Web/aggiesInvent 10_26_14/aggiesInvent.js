///////////////////////////////////////////////////////////////////////////////
/// Location Collection
Location = new Meteor.Collection("location");
Logs = new Meteor.Collection("logs");


/// //////////////////////////////////////////////////////////////////////////
//// Meteor Client
if (Meteor.isClient) {

    ///////////////////////////////////////////////////////////////////////////
    ///// Google Map Template

    Template.googleMap.helpers({
        rendered : function () {

            $(window).resize(function(){
                $('#map_canvas').css("height",$(window).height());
                $('#map_canvas').css("width",$(window).width());
                google.maps.event.trigger(map, 'resize');
                map.setZoom( map.getZoom() );
            });

            var mapOptions = {
                zoom: 12,
                center: new google.maps.LatLng(30.627977,-96.334407),
                mapTypeControlOptions: {
                    mapTypeIds: [google.maps.MapTypeId.TERRAIN, 'map_style']
                }
            };

            // Add Map To Canvas Element
            map = new google.maps.Map(document.getElementById('map-canvas'),
                mapOptions);

            // AutoRun Function To Constantly Add Markers To Map
            Deps.autorun( function () {
                // Get Collection With Map Items
                Location.find().forEach(function (entry) {
                    // Log Entry Values
                    console.log(entry.latitude, entry.longitude);
                    // Add Marker To Map
                    marker = new google.maps.Marker({
                        position: new google.maps.LatLng(entry.latitude, entry.longitude),
                        map: map,
                        title: entry.name,
                        animation: google.maps.Animation.DROP
                    });

                    // Deletes Old Google Map Pin
                    Location.find().observeChanges({
                        changed: function(id, doc) {
                            marker.setMap(null);
                        }
                    });
                });
            })
        }
    });

    Template.googleMap.events({
        'click #moveMarker' : function () {
            var loc = Location.findOne();
            Location.update(loc._id, {$set:{latitude:loc.latitude + 0.005, longitude:loc.longitude + 0.005}});
        }
    });


    Template.logEntry.helpers({
        logHit : function () {
            return Logs.find();
        }
    })


}




//////////////////////////////////////////////////////////////////////
//// Server Implementation
if (Meteor.isServer) {

    /// Intial insert of location
    Meteor.startup(function () {
        if (Location.find().count() === 0) {
           Location.insert({latitude: 30.627977, longitude: -96.334407});
        }
    });

    /// HTTP Post Methods
    HTTP.methods({
        'test': function() {
            console.log("PINGED");
            var d = new Date();
            var n = d.getTime();
            Logs.insert({logs: "PINGED AT TIME " + n });
//            var parsedString =  data.toString.split("&");
//            console.log("latitude " + parsedString[0]);
//            console.log("latitude " + parsedString[1]);
//            var string = data.toString().split("&");
//            console.log(string[0]);
//            console.log(string[1]);
//            Location.update(loc._id, {$set:{latitude:loc.latitude + 0.005, longitude:loc.longitude + 0.005}});

        }
    });

}
