// Lấy các Element 
const ssidElement = document.querySelector("[name='ssid']"); 
const passElement = document.querySelector("[name='pass']");
const btnUpdateWiFiInfo = document.querySelector(".btn_update");

console.log(btnUpdateWiFiInfo)
// Function lấy data gửi từ esp32 lên
const getDataConfigFromServer = () => {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "wifi_info", false);

    xhttp.onreadystatechange = () => {
        if (xhttp.readyState == 4 && xhttp.status == 200) {
            const json_raw = xhttp.responseText;

            const json_data = JSON.parse(json_raw);

                if(ssidElement && passElement) {
                    ssidElement.value = json_data.ssid;
                    passElement.value = json_data.pass;
                }
        }
    }
    xhttp.send();
}

// Function update wifi, submit gửi data cho esp32
const updateWiFiInfo = () => {
    btnUpdateWiFiInfo.addEventListener("click", () => {
        const obj = {
            wifi_info: {
                ssid: ssidElement.value,
                pass: passElement.value
            }
        };

        // convert json to string
        const jsonString = JSON.stringify(obj);

        var xhttp = new XMLHttpRequest();

        xhttp.onreadystatechange = async () => {
            if (xhttp.readyState == 4 && xhttp.status == 200) {
                const response = xhttp.responseText;
                alert(response);
            }
        };

        xhttp.open("POST", "/wifi_update", true);
        xhttp.setRequestHeader("Content-type", "application/json");
        xhttp.send(jsonString);
    });
}

getDataConfigFromServer();
updateWiFiInfo();