function userLogin(username) {
    var gotohere = username + ".html";
    location.href = gotohere;
}

function runWasher(setting) {
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "RunWasher";
    requestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(requestURL, setting, function() {
        alert("Success!");
        //Right now this handler will return you to the homepage on a successfull POST, do we want that?
        location.href = "home.html"
    });
}

function runDryer(setting) {
    var deviceID = "2f0059000e51353532343635";
    var accessToken = "4dcf12c1f71b9601b0e988744eedbf79c14252af";
    var baseURL = "https://api.particle.io/v1/devices/";
    var funcName = "RunDryer";
    requestURL = baseURL + deviceID + "/" + funcName + "/?access_token=" + accessToken;

    $.post(requestURL, setting, function() {
        alert("Success!");
        //same thing here
        location.href = "home.html";
    });
}

//function createCycle()