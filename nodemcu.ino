#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHHTYPE DHT11
WiFiClient client;  // client is a variable
WiFiServer server(80);  // server is a variable. port 80 is the port that the server listens to
#define Analog_pin A0
int val=0;
int Board_Resolution=1024;
#define led D5 // 14
#define fan D2
#define dht_pin D1
DHT dht(dht_pin, DHHTYPE);

void setup() {
  dht.begin();
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);
  Serial.begin(9600);
  WiFi.begin("X-Hack 2.0", "GoodLuck10");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("0");
    delay(300);
  }
  Serial.println(" ");
  Serial.println("Wifi Conntection Succesful");
  Serial.println(WiFi.localIP());
  delay(3000);
  server.begin();  // making the NodeMCU a server
  pinMode(led, OUTPUT);
}

String prepareHtmlPage(float temp,float Humidity)
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 1\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE html>"
"<html lang='en'>"
"<head>"
  "<meta charset='utf-8'>"
  "<meta content='width=device-width, initial-scale=1.0' name='viewport'>"

  "<title>Greem House Monitoring System</title>"
  "<link href='https://fonts.gstatic.com' rel='preconnect'>"
  "<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,300i,400,400i,600,600i,700,700i|Nunito:300,300i,400,400i,600,600i,700,700i|Poppins:300,300i,400,400i,500,500i,600,600i,700,700i' rel='stylesheet'>"
  "<!-- Vendor CSS Files -->"
  "<link href='https://premstha.com.np/assets/vendor/bootstrap/css/bootstrap.min.css' rel='stylesheet'>"
  "<!-- Include Bootstrap Icons CSS -->"
  "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css'>"
  "<link href='https://premstha.com.np/assets/vendor/quill/quill.snow.css' rel='stylesheet'>"
  "<!-- Template Main CSS File -->"
  "<link href='https://premstha.com.np/assets/css/dashboard-style.css' rel='stylesheet'>"
"</head>"
"<body>"
  "<!-- ======= Header ======= -->"
  "<header id='header' class='header fixed-top d-flex align-items-center'>"

    "<div class='d-flex align-items-center justify-content-between'>"
      "<a href='index.html' class='logo d-flex align-items-center'>"
        "<img src='https://premstha.com.np/assets/img/logo.png' alt=''>"
        "<span class='d-none d-lg-block'>PREMSTHA</span>"
      "</a>"
      "<i class='bi bi-list toggle-sidebar-btn'></i>"
    "</div><!-- End Logo -->"

      "</ul>"

  "</header><!-- End Header -->"

  "<!-- ======= Sidebar ======= -->"
  "<aside id='sidebar' class='sidebar'>"

    "<ul class='sidebar-nav' id='sidebar-nav'>"

      "<li class='nav-item'>"
        "<a class='nav-link' href='#'>"
          "<i class='bi bi-grid'></i>"
          "<span>Dashboard</span>"
        "</a>"
      "</li><!-- End Dashboard Nav -->"
      "</ul>"

  "</aside><!-- End Sidebar-->"

  "<main id='main' class='main'>"

    "<div class='pagetitle'>"
      "<h1>Green House Monitoring and Automation System</h1>"
    "</div><!-- End Page Title -->"

    "<section class='section dashboard'>"
      "<div class='row'>"
      "<div class='col-lg-12'>"
          "<div class='row'>"
            "<!-- Light card -->"
            "<div class='col-xxl-4 col-md-6'>"
              "<div class='card info-card text-warning'>"
                "<div class='card-body'>"
                  "<h5 class='card-title'>Light</h5>"

                  "<div class='d-flex align-items-center'>"
                    "<div class='card-icon rounded-circle d-flex align-items-center justify-content-center'>"
                      "<i class='bi bi-lightbulb'></i>"
                    "</div>"
                    "<div class='ps-3'>"
                      "<h6 class='status'></h6>"
                      "<a class='btn btn-success' href='./ledon' role='button'>ON</a>"
                      "<a class='btn btn-danger' href='./ledoff' role='button'>OFF</a>"
                    "</div>"
                  "</div>"
                "</div>"

              "</div>"
            "</div><!-- End light Card -->"
            
            "<!-- Light card -->"
            "<div class='col-xxl-4 col-md-6'>"
              "<div class='card info-card text-warning'>"
                "<div class='card-body'>"
                  "<h5 class='card-title'>Fan</h5>"

                  "<div class='d-flex align-items-center'>"
                    "<div class='card-icon rounded-circle d-flex align-items-center justify-content-center'>"
                      "<i class='bi bi-fan'></i>"
                    "</div>"
                    "<div class='ps-3'>"
                      "<h6 class='status'></h6>"
                      "<a class='btn btn-success' href='/fanon' role='button'>ON</a>"
                      "<a class='btn btn-danger' href='./fanoff' role='button'>OFF</a>"
                    "</div>"
                  "</div>"
                "</div>"

              "</div>"
            "</div><!-- End light Card -->"
          "</div>"
        "</div>"

        "<!-- Left side columns -->"
        "<div class='col-lg-12'>"
          "<div class='row'>"

            "<!-- Temperature Card -->"
            "<div class='col-xxl-4 col-md-6'>"
              "<div class='card info-card sales-card'>"

                "<div class='card-body'>"
                  "<h5 class='card-title'>Temperature</h5>"

                  "<div class='d-flex align-items-center'>"
                    "<div class='card-icon rounded-circle d-flex align-items-center justify-content-center'>"
                      "<i class='bi bi-thermometer'></i>"
                    "</div>"
                    "<div class='ps-3'>"
                      "<h6>");
                      htmlPage += String(temp);
                      htmlPage += F(
                      "</h6>"
                    "</div>"
                  "</div>"
                "</div>"

              "</div>"
            "</div><!-- End Temperature Card -->"

            "<!-- Humidity Card -->"
            "<div class='col-xxl-4 col-md-6'>"
              "<div class='card info-card sales-card'>"

                "<div class='card-body'>"
                  "<h5 class='card-title'>Humidity</h5>"

                  "<div class='d-flex align-items-center'>"
                    "<div class='card-icon rounded-circle d-flex align-items-center justify-content-center'>"
                      "<i class='bi bi-thermometer'></i>"
                    "</div>"
                    "<div class='ps-3'>"
                      "<h6>");
                      htmlPage += String(Humidity);
                      htmlPage += F("</h6>"
                    "</div>"
                  "</div>"
                "</div>"

              "</div>"
            "</div><!-- End Humidity Card -->"

//             "<!-- Reports -->"
//             "<div class='col-12'>"
//               "<div class='card'>"

//                 "<div class='card-body'>"
//                   "<h5 class='card-title'>Overall performance <span>/Today</span></h5>"
//                   "<!-- Line Chart -->"
//                   "<div id='reportsChart'></div>"
//                   "<script>"
//                   "document.addEventListener('DOMContentLoaded', () => {"
//   "const dates = <?php echo json_encode($date_array); ?>;"
//   "const percent = <?php echo json_encode($percent); ?>;"
//   "const comPercent = <?php echo json_encode($com_percent); ?>;"

//   "if (dates.length === 0) {"
//     "var message = document.createElement('div');"
//     "message.innerHTML = 'No items to display';"
//     "message.style.fontSize = '18px';"
//     "message.style.color = '#94a3b8';"
//     "message.style.textAlign = 'center';"
//     "document.querySelector('#reportsChart').appendChild(message);"
//   "} else {"
//     "const maxLabels = 7;"
//     "const interval = Math.ceil(dates.length / maxLabels);"
//     "let categories = dates.map((date, index) => (index % interval === 0 ? date : ''));"

//     "new ApexCharts(document.querySelector('#reportsChart'), {"
//       "series: ["
//         "{ name: 'Your performance', data: percent },"
//         "{ name: 'Your competitors performance', data: comPercent }"
//       "],"
//       "chart: {"
//         "height: 350,"
//         "type: 'area',"
//         "toolbar: {"
//           "show: false"
//         "}"
//       "},"
//       "markers: {"
//         "size: 4"
//       "},"
//       "colors: ['#2eca6a', '#4154f1'],"
//       "fill: {"
//         "type: 'gradient',"
//         "gradient: {"
//           "shadeIntensity: 1,"
//           "opacityFrom: 0.3,"
//           "opacityTo: 0.4,"
//           "stops: [0, 90, 100]"
//         "}"
//       "},"
//       "dataLabels: {"
//         "enabled: 'false'"
//       "},"
//       "stroke: {"
//         "curve: 'smooth',"
//         "width: 2"
//       "},"
//       "xaxis: {"
//         "type: 'category',"
//         "categories: categories,"
//         "labels: {"
//           "show: true,"
//           "rotate: -45,"
//           "rotateAlways: false,"
//           "hideOverlappingLabels: false,"
//           "trim: false,"
//           "minHeight: undefined,"
//           "maxHeight: 120,"
//           "style: {"
//             "colors: [],"
//             "fontSize: '12px',"
//             "fontFamily: 'Helvetica, Arial, sans-serif',"
//             "cssClass: 'apexcharts-xaxis-label',"
//           "},"
//         "}"
//       "},"
//       "yaxis: {"
//         "labels: {"
//           "formatter: function (value) {"
//             "// Format the value to display with two decimal places"
//             "return value.toFixed(2);"
//           "}"
//         "}"
//       "},"
//       "tooltip: {"
//         "x: {"
//           "format: 'dd/MM/yy HH:mm'"
//         "}"
//       "},"
//       "noData: {"
//         "text: 'No data to display. Try some practice sets, mock tests, etc. to see analytics',"
//         "fontColor: '#ddd',"
//         "fontSize: 18"
//       "}"
//     "}).render();"
//   "}"
// "});"
//                   "</script>"
//                   "<!-- End Line Chart -->"

//                 "</div>"

//               "</div>"
//             "</div><!-- End Reports -->"
          "</div>"
        "</div><!-- End Left side columns -->"

        "</div>"
    "</section>"

  "</main><!-- End #main -->"

  "<!-- ======= Footer ======= -->"
  "<footer id='footer' class='footer'>"
    "<div class='copyright'>"
      "&copy; Copyright <strong><span>IOEMENTOR</span></strong>. All Rights Reserved"
    "</div>"
  "</footer><!-- End Footer -->"

  "<a href='#' class='back-to-top d-flex align-items-center justify-content-center'><i class='bi bi-arrow-up-short'></i></a>"

  "<!-- Vendor JS Files -->"
  "<script src='https://premstha.com.np/assets/vendor/apexcharts/apexcharts.min.js'></script>"
  "<script src='https://premstha.com.np/assets/vendor/bootstrap/js/bootstrap.bundle.min.js'></script>"

  "<script src='https://premstha.com.np/assets/vendor/echarts/echarts.min.js'></script>"
  "<script src='https://premstha.com.np/assets/vendor/simple-datatables/simple-datatables.js'></script>"
  "<script src='https://premstha.com.np/assets/vendor/tinymce/tinymce.min.js'></script>"

  "<!-- Template Main JS File -->"
  "<script src='https://premstha.com.np/assets/js/dashboard-main.js'></script>"

"</body>"
"<script src='https://code.jquery.com/jquery-3.6.0.min.js'></script>"
"<div id='notifications'>"
    "<!-- Notifications will be displayed here -->"
"</div>"


"</html>"
  "\r\n");
  return htmlPage;
}

void loop() {
  val=analogRead(Analog_pin);
  // float voltage=val;
  //Serial.println("Serial Communication enabled");
  Serial.println(val);
  if(val<700){
     digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print("\n ");
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println("C \n \n \n ");
  // delay(1000);
  if(t > 21){
      digitalWrite(fan, HIGH);
    }
    if(t<21){
       digitalWrite(fan, LOW);
    }
  client = server.available();  // if available any client to the server, client becomes 1, and it means that the client has requested something.
  if (client) {
    String request = client.readStringUntil('\n');  // reading the request from the client
    Serial.println(request);
    request.trim();
    if (request == "GET /ledon HTTP/1.1") {
      digitalWrite(led, HIGH);
    }
    if (request == "GET /ledoff HTTP/1.1") {
      digitalWrite(led, LOW);
      exit(0);
    }
    if (request == "GET /fanon HTTP/1.1") {
      digitalWrite(fan, HIGH);
    }
    
    if (request == "GET /fanoff HTTP/1.1") {
      digitalWrite(fan, LOW);
      exit(0);
    }
    client.println(prepareHtmlPage(t,h));
    client.stop();
  }
}