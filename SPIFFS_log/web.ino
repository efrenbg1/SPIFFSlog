void handleRoot() {
  server.send(200, "text/html", htmlListLogs(SPIFFS));
}

void handleGetFile() {
  String path = server.arg("file");
  Serial.printf("Reading file: %s\r\n", path.c_str());
    File file = SPIFFS.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }
    server.streamFile(file,"text/plain");
    file.close();
}
