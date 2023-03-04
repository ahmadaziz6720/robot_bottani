// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyABTMahcC3vLi7MvSOj04RqMBb1p-rFj-8",
    authDomain: "approkes-89937.firebaseapp.com",
    databaseURL: "https://approkes-89937-default-rtdb.firebaseio.com",
    projectId: "approkes-89937",
    storageBucket: "approkes-89937.appspot.com",
    messagingSenderId: "1053571501098",
    appId: "1:1053571501098:web:973d9d46b2d659174dc753"
  };


  
  // Initialize Firebase
firebase.initializeApp(firebaseConfig);

hilangkan("realtime");
hilangkan("form");
hilangkan("edit");
hilangkan("notAvailable");

munculkan("controller");

function munculkan(id){
    var elem = document.getElementById(id);
    if (elem.style.display === "none") {
        elem.style.display = "block";
    } else {
        elem.style.display = "none";
    }
}

function hilangkan(){
    var controller = document.getElementById("controller");
    // var form = document.getElementById("form");
    // var edit = document.getElementById("edit");
    var notAvailable = document.getElementById("notAvailable");
    var realtime = document.getElementById("realtime");

    controller.style.display = "none";
    // form.style.display = "none";
    // edit.style.display = "none";
    notAvailable.style.display = "none";
    realtime.style.display = "none";
}

function updateRules() {
    munculkan("edit");
    munculkan("form");
    munculkan("rules");

    var capacity = document.getElementById("capacity").value;
    var max_tmp = document.getElementById("max_tmp").value;

    firebase.database().ref("/").child("rules").update({
        Capacity: capacity,
        MaxTemp : max_tmp
    })

    document.getElementById("capacity2").innerHTML = capacity;
    document.getElementById("max_tmp2").innerHTML = max_tmp;
}
setInterval(function getData(){
    firebase.database().ref("/Dashboard").once('value').then(function (snapshot) {
        var ActiveGate = snapshot.val().ActiveGate;
        var NoMask = snapshot.val().NoMask;
        var OverTemperature = snapshot.val().OverTemperature;
        var People = snapshot.val().People;
        var PeopleEntered = snapshot.val().PeopleEntered;
        var PeopleOut = snapshot.val().PeopleOut;

        document.getElementById("ActiveGate").innerHTML = ActiveGate;
        document.getElementById("NoMask").innerHTML = NoMask;
        document.getElementById("OverTemperature").innerHTML = OverTemperature;
        document.getElementById("People").innerHTML = People;
        document.getElementById("PeopleEntered").innerHTML = PeopleEntered;
        document.getElementById("PeopleOut").innerHTML = PeopleOut;
    });
}, 1000);