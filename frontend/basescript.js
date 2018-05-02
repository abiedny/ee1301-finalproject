function userLogin(username) {
    var gotohere = username + ".html";
    location.href = gotohere;
}

function stopWasher() {
    var setting = "1";
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "StopWasher";
    stopWrequestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(stopWrequestURL, {"args":setting}, function() {
        alert("Washer stopped.");
        //Right now this handler will return you to the homepage on a successfull POST, do we want that?
    });
}

function runWasher() {
    setting = document.getElementById('cyclelist').value;
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "RunWasher";
    runWrequestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(runWrequestURL, {"args":setting}, function() {
        alert("Success!");
        location.href = "home.html";
    });
}
function runWasherPreset(setting) {
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "RunWasher";
    runWPrequestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(runWPrequestURL, {"args":setting}, function() {
        alert("Success!");
        location.href = "home.html";
    });
}

function stopDryer() {
    var setting = "1";
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "StopDryer";
    stopDrequestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(stopDrequestURL, {"args":setting}, function() {
        alert("Dryer stopped.");
    });
}

function runDryer() {
    setting = document.getElementById('cyclelist').value;
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "RunDryer";
    runDrequestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(runDrequestURL, {"args":setting}, function() {
        alert("Success!");
        //same thing here
        location.href = "home.html";
    });
}
function runDryerPreset(setting) {
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "RunDryer";
    runDPrequestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(runDPrequestURL, {"args":setting}, function() {
        alert("Success!");
        //same thing here
        location.href = "home.html";
    });
}
function popupCycleBox() {
    var modal = document.getElementById('createcyclebox');
    var span = document.getElementsByClassName("close")[0];
    span.onclick = function() {
        modal.style.display = "none";
    }
    modal.style.display = "block";
}
//I can hardly believe it but this shit is working?!
//uses cookies to save cycles, see other js files
function createCycle() {
    var type = document.forms[0].elements.namedItem("type").value;
    var name = document.forms[0].elements.namedItem("cyclename").value;
    var speed = document.forms[0].elements.namedItem("speed").value;
    var time = document.forms[0].elements.namedItem("time").value;
    var temp = document.forms[0].elements.namedItem("temp").value
    var option = document.createElement("option");
    option.innerHTML = name;
    if (type === "dryer") {
        option.value = name + " " + speed + " " + time; //Whatever form the post string needs to be in, because value is the post data
    }
    else if (type === "washer") {
        option.value = name + " " + speed + " " + time + " " + temp;
    }
    $("#cyclelist").append(option);
    //The cookies for each created cycle are named the location of the user page, and data is the option variable
    if ((getCookie(location.href + "zero") != null) && (getCookie(location.href + "one") === null)) {
        document.cookie = location.href + "one" + "=" + option.value;
    }
    else if ((getCookie(location.href + "zero") != null) && (getCookie(location.href + "one") != null)) {
        document.cookie = location.href + "two" + "=" + option.value;
    }
    else {
        document.cookie = location.href + "zero" + "=" + option.value;
    }
    document.getElementById('createcyclebox').style.display = "none";
}

//replaces the <p> tags under washer and dryer
//any post or json crap needs to be tested
$(document).ready(function() {
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";

    //ALL OF THESE NEED TO BE CLOUD VARIABLES AND ADDED TO THE C++
    //Washer time left
    WRrequestURL = baseURL + deviceID + "/" + "WTimeLeft" + "/?access_token=" + accessToken;
    setInterval(function() {
        $.getJSON(WRrequestURL, function(json) {
            $("#washerdata").html("Time left: " + json.result);
        })}, 900);

    //Dryer time left
    DRrequestURL = baseURL + deviceID + "/" + "DTimeLeft" + "/?access_token=" + accessToken;
    setInterval(function() {
        $.getJSON(DRrequestURL, function(json) {
            $("#dryerdata").html("Time left: " + json.result);
        })}, 900);

    //Current washer cycle
    WCrequestURL = baseURL + deviceID + "/" + "WasherCycle" + "/?access_token=" + accessToken;
    setInterval(function() {
        $.getJSON(WCrequestURL, function(json) {
            $("#washercycle").html("Current Cycle: " + json.result);
        })}, 900);
    
    //Current dryer cycle
    DCrequestURL = baseURL + deviceID + "/" + "DryerCycle" + "/?access_token=" + accessToken;
    setInterval(function() {
        $.getJSON(DCrequestURL, function(json) {
            $("#dryercycle").html("Current Cycle: " + json.result);
        })}, 900);

});