//Everything here is working, only for one cookie tho
//Persists as long as site isn't closed and reopened, thats probably because I am on localhost tho and it can't fetch the cookie??
function getCookie(name) {
    var dc = document.cookie;
    var prefix = name + "=";
    var begin = dc.indexOf("; " + prefix);
    if (begin == -1) {
        begin = dc.indexOf(prefix);
        if (begin != 0) return null;
    }
    else
    {
        begin += 2;
        var end = document.cookie.indexOf(";", begin);
        if (end == -1) {
        end = dc.length;
        }
    }
    return decodeURI(dc.substring(begin + prefix.length, end));
}

$(document).ready(function() {
    if (getCookie(location.href) != null) {
        var option = document.createElement("option");
        option.value = getCookie(location.href);
        var name = option.value.substr(0, option.value.indexOf(" "));
        option.innerHTML = name;
        $("#cyclelist").append(option);
    }
});