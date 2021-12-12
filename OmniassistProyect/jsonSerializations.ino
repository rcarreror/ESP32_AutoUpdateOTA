void createEmptyJson() {
  confJSON["configured"] = false;
  confJSON["nombreCliente"] = "";
  confJSON["nombreAsistente"] = "";
  confJSON["idAsistente"] = "423526462";

  JsonArray pastillas = confJSON.createNestedArray("pastillas");

  JsonObject pastillas_0 = pastillas.createNestedObject();
  pastillas_0["nombrePastilla"] = "";
  pastillas_0["tipoPastilla"] = "";
  pastillas_0["frecuencia"] = 0;

  JsonArray pastillas_0_dosisDiaria = pastillas_0.createNestedArray("dosisDiaria");

  JsonObject pastillas_0_dosisDiaria_0 = pastillas_0_dosisDiaria.createNestedObject();
  pastillas_0_dosisDiaria_0["necesaria"] = false;
  pastillas_0_dosisDiaria_0["horaToma"] = "";

  JsonObject pastillas_0_dosisDiaria_1 = pastillas_0_dosisDiaria.createNestedObject();
  pastillas_0_dosisDiaria_1["necesaria"] = false;
  pastillas_0_dosisDiaria_1["horaToma"] = "";

  JsonObject pastillas_0_dosisDiaria_2 = pastillas_0_dosisDiaria.createNestedObject();
  pastillas_0_dosisDiaria_2["necesaria"] = false;
  pastillas_0_dosisDiaria_2["horaToma"] = "";
  pastillas_0["ultimasTomas"][0] = "";

  JsonObject pastillas_1 = pastillas.createNestedObject();
  pastillas_1["nombrePastilla"] = "";
  pastillas_1["tipoPastilla"] = "";
  pastillas_1["frecuencia"] = 0;

  JsonArray pastillas_1_dosisDiaria = pastillas_1.createNestedArray("dosisDiaria");

  JsonObject pastillas_1_dosisDiaria_0 = pastillas_1_dosisDiaria.createNestedObject();
  pastillas_1_dosisDiaria_0["necesaria"] = false;
  pastillas_1_dosisDiaria_0["horaToma"] = "";

  JsonObject pastillas_1_dosisDiaria_1 = pastillas_1_dosisDiaria.createNestedObject();
  pastillas_1_dosisDiaria_1["necesaria"] = false;
  pastillas_1_dosisDiaria_1["horaToma"] = "";

  JsonObject pastillas_1_dosisDiaria_2 = pastillas_1_dosisDiaria.createNestedObject();
  pastillas_1_dosisDiaria_2["necesaria"] = false;
  pastillas_1_dosisDiaria_2["horaToma"] = "";
  pastillas_1["ultimasTomas"][0] = "";

  JsonObject pastillas_2 = pastillas.createNestedObject();
  pastillas_2["nombrePastilla"] = "";
  pastillas_2["tipoPastilla"] = "";
  pastillas_2["frecuencia"] = 0;

  JsonArray pastillas_2_dosisDiaria = pastillas_2.createNestedArray("dosisDiaria");

  JsonObject pastillas_2_dosisDiaria_0 = pastillas_2_dosisDiaria.createNestedObject();
  pastillas_2_dosisDiaria_0["necesaria"] = false;
  pastillas_2_dosisDiaria_0["horaToma"] = "";
  pastillas_2_dosisDiaria_0["ultimaToma"] = "";

  JsonObject pastillas_2_dosisDiaria_1 = pastillas_2_dosisDiaria.createNestedObject();
  pastillas_2_dosisDiaria_1["necesaria"] = false;
  pastillas_2_dosisDiaria_1["horaToma"] = "";
  pastillas_2_dosisDiaria_1["ultimaToma"] = "";

  JsonObject pastillas_2_dosisDiaria_2 = pastillas_2_dosisDiaria.createNestedObject();
  pastillas_2_dosisDiaria_2["necesaria"] = false;
  pastillas_2_dosisDiaria_2["horaToma"] = "";
  pastillas_2_dosisDiaria_2["ultimaToma"] = "";
  pastillas_2["ultimasTomas"][0] = "";

  EepromStream eepromStream(0, 1536);
  serializeJson(confJSON, eepromStream);
  eepromStream.flush();
}

void createTestJson() {
  confJSON["configured"] = true;
  confJSON["nombreCliente"] = "Paco Benancio Ramirez";
  confJSON["nombreAsistente"] = "Francisco Ramirez";
  confJSON["idAsistente"] = "423526462";

  JsonArray pastillas = confJSON.createNestedArray("pastillas");

  JsonObject pastillas_0 = pastillas.createNestedObject();
  pastillas_0["nombrePastilla"] = "Ameride";
  pastillas_0["tipoPastilla"] = "antihipertensivo";
  pastillas_0["frecuencia"] = 8;

  JsonArray pastillas_0_dosisDiaria = pastillas_0.createNestedArray("dosisDiaria");

  JsonObject pastillas_0_dosisDiaria_0 = pastillas_0_dosisDiaria.createNestedObject();
  pastillas_0_dosisDiaria_0["necesaria"] = true;
  pastillas_0_dosisDiaria_0["horaToma"] = "6:00";

  JsonObject pastillas_0_dosisDiaria_1 = pastillas_0_dosisDiaria.createNestedObject();
  pastillas_0_dosisDiaria_1["necesaria"] = true;
  pastillas_0_dosisDiaria_1["horaToma"] = "14:00";

  JsonObject pastillas_0_dosisDiaria_2 = pastillas_0_dosisDiaria.createNestedObject();
  pastillas_0_dosisDiaria_2["necesaria"] = true;
  pastillas_0_dosisDiaria_2["horaToma"] = "22:00";

  JsonArray pastillas_0_ultimasTomas = pastillas_0.createNestedArray("ultimasTomas");
  pastillas_0_ultimasTomas.add("06:05");
  pastillas_0_ultimasTomas.add("14:10");
  pastillas_0_ultimasTomas.add("22:01");

  JsonObject pastillas_1 = pastillas.createNestedObject();
  pastillas_1["nombrePastilla"] = "Veratran";
  pastillas_1["tipoPastilla"] = "ansiolitico";
  pastillas_1["frecuencia"] = 12;

  JsonArray pastillas_1_dosisDiaria = pastillas_1.createNestedArray("dosisDiaria");

  JsonObject pastillas_1_dosisDiaria_0 = pastillas_1_dosisDiaria.createNestedObject();
  pastillas_1_dosisDiaria_0["necesaria"] = true;
  pastillas_1_dosisDiaria_0["horaToma"] = "6:00";

  JsonObject pastillas_1_dosisDiaria_1 = pastillas_1_dosisDiaria.createNestedObject();
  pastillas_1_dosisDiaria_1["necesaria"] = true;
  pastillas_1_dosisDiaria_1["horaToma"] = "18:00";

  JsonObject pastillas_1_dosisDiaria_2 = pastillas_1_dosisDiaria.createNestedObject();
  pastillas_1_dosisDiaria_2["necesaria"] = false;
  pastillas_1_dosisDiaria_2["horaToma"] = "";

  JsonArray pastillas_1_ultimasTomas = pastillas_1.createNestedArray("ultimasTomas");
  pastillas_1_ultimasTomas.add("06:06");
  pastillas_1_ultimasTomas.add("18:05");

  JsonObject pastillas_2 = pastillas.createNestedObject();
  pastillas_2["nombrePastilla"] = "Tramadol";
  pastillas_2["tipoPastilla"] = "analgesico";
  pastillas_2["frecuencia"] = 24;

  JsonArray pastillas_2_dosisDiaria = pastillas_2.createNestedArray("dosisDiaria");

  JsonObject pastillas_2_dosisDiaria_0 = pastillas_2_dosisDiaria.createNestedObject();
  pastillas_2_dosisDiaria_0["necesaria"] = true;
  pastillas_2_dosisDiaria_0["horaToma"] = "6:00";
  pastillas_2_dosisDiaria_0["ultimaToma"] = "06:03";

  JsonObject pastillas_2_dosisDiaria_1 = pastillas_2_dosisDiaria.createNestedObject();
  pastillas_2_dosisDiaria_1["necesaria"] = false;
  pastillas_2_dosisDiaria_1["horaToma"] = "";
  pastillas_2_dosisDiaria_1["ultimaToma"] = "";

  JsonObject pastillas_2_dosisDiaria_2 = pastillas_2_dosisDiaria.createNestedObject();
  pastillas_2_dosisDiaria_2["necesaria"] = false;
  pastillas_2_dosisDiaria_2["horaToma"] = "";
  pastillas_2_dosisDiaria_2["ultimaToma"] = "";
  pastillas_2["ultimasTomas"][0] = "06:03";

  EepromStream eepromStream(0, 1536);
  serializeJson(confJSON, eepromStream);
  eepromStream.flush();
}
