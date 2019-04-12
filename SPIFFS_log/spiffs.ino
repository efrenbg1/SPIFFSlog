String htmlListLogs(fs::FS &fs){
    int freeSize = 0;
    String html = "<!DOCTYPE html>\n<html>\n<body>\n<h1>Index of /</h1>\n<br><button type=\"button\" onclick=\"var arr = [], l = document.links;for(var i=0; i<l.length; i++) {l[i].click();console.log(l[i].href);}\">Download all</button><br><br><ul>\n"; 
    File root = fs.open("/");
    if(!root){
        Serial.println("- failed to open directory");
        return "Error openning directory!";
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return "Error openning directory!";
    }
    String list = "";
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("Skipping directory of name: ");
            Serial.println(file.name());
            freeSize = freeSize + file.size();
        } else {
            String sFile = file.name();
            freeSize = freeSize + file.size();
            int fileSize = file.size()/1024;
            html = html + "<li><a target=\"_blank\" download=\"" + sFile.substring(1) +  "\" href=\"get?file=" + sFile + "\">" + sFile.substring(1) + "</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + fileSize + " KB</li>\n";
        }
        file = root.openNextFile();
    }
    freeSize = SPIFFS.totalBytes()-freeSize;
    float avaSize = float(freeSize)/(1024*1024);
    html = html + "</ul>\n<br><br> Free space: " + avaSize + " MB</body>\n<script>function all(){var arr = [], l = document.links;for(var i=0; i<l.length; i++) {l[i].click();}}</script></html>";
    return html;
}

void logData(String str){
  if(doneMax){
    Serial.println("Memory full!");
    return;
  }
  if(logNum < 0){
    File root = SPIFFS.open("/");
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("Skipping directory of name: ");
            Serial.println(file.name());
        } else {
          String sFile = file.name();
            if(sFile.substring(0,4) == "/log" && (sFile.substring(4,5)).toInt() > logNum){
              logNum = (sFile.substring(4,5)).toInt();
            }
            }
        file = root.openNextFile();
  }
  logNum++;
  String fileName = "/log";
  fileName += logNum;
  fileName = fileName + ".txt";
  writeFile(SPIFFS, fileName.c_str(), str.c_str());
  return;
  }
  
  String fileName = "/log";
  fileName += logNum;
  fileName = fileName + ".txt";
  File file = SPIFFS.open(fileName);
  
    //// Check for max size ////
    if(file.size() >= (maxFileSize)){
      //checkMax();
      if(logNum == -2){
        return;
      }
      logNum++;
    }
    file = SPIFFS.open(fileName, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(str)){
    } else {
        Serial.println("- append failed, memory full?");
        doneMax = false;
    }  
    file.close();  
}

String getFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "Error openning";
    }
    String contents;
    while(file.available()){
      contents += (char)file.read();
    }
    contents += '\0';
    file.close();
    return contents;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
        doneMax = true;
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
