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

//if the cookies ain't null, then load 'em
$(document).ready(function() {
    if (getCookie(location.href + "zero") != null) {
        var option = document.createElement("option");
        option.value = getCookie(location.href + "zero");
        var name = option.value.substr(0, option.value.indexOf(" "));
        option.innerHTML = name;
        $("#cyclelist").append(option);
    }

    if (getCookie(location.href + "one") != null) {
        var option = document.createElement("option");
        option.value = getCookie(location.href + "one");
        var name = option.value.substr(0, option.value.indexOf(" "));
        option.innerHTML = name;
        $("#cyclelist").append(option);
    }

    if (getCookie(location.href + "two") != null) {
        var option = document.createElement("option");
        option.value = getCookie(location.href + "two");
        var name = option.value.substr(0, option.value.indexOf(" "));
        option.innerHTML = name;
        $("#cyclelist").append(option);
    }
});