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
    // firebase.database().ref("/Dashboard").once('value').then(function (snapshot) {
        // var ActiveGate = snapshot.val().ActiveGate;
        // var NoMask = snapshot.val().NoMask;
        // var OverTemperature = snapshot.val().OverTemperature;
        // var People = snapshot.val().People;
        // var PeopleEntered = snapshot.val().PeopleEntered;
        // var PeopleOut = snapshot.val().PeopleOut;
    var Temperature = parseFloat(22 + Math.random() * 0.2).toFixed(2)
    var Moisture = parseFloat(33.84 + Math.random() * 0.01).toFixed(3)
    var pH = parseFloat(6.7 + Math.random()*0.005).toFixed(1)
    
    document.getElementById("Moisture").innerHTML = Moisture;
    document.getElementById("Temperature").innerHTML = Temperature;
    document.getElementById("pH").innerHTML = pH;
    document.getElementById("NPK").innerHTML = "4:2:1";
}, 1000);