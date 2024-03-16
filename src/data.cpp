#include "data.hpp"

const String mempoolInstance = "https://mempool.space";

const String mempoolPriceApiUrl = mempoolInstance + "/api/v1/prices";
const String mempoolBlockApiUrl = mempoolInstance + "/api/blocks/tip/height";
const String mempoolFeeApiUrl = mempoolInstance + "/api/v1/fees/recommended";

uint lastPrice;
uint lastBlock;

uint getPrice()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(mempoolPriceApiUrl);

    int httpCode = http.GET();

    // Parse JSON response and extract average price
    uint usdPrice, eurPrice;
    if (httpCode == 200)
    {
        String payload = http.getString();
        JsonDocument doc;
        deserializeJson(doc, payload);

        lastPrice = doc[preferences.getString(SETTING_CURRENCY)].as<uint>();

        return lastPrice;
    }
    else
    {
        Serial.printf("HTTP GET request price failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return 0;
}

uint getBlock()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(mempoolBlockApiUrl);

    int httpCode = http.GET();

    // Parse JSON response and extract average price
    uint usdPrice, eurPrice;
    if (httpCode == 200)
    {
        uint payload = http.getString().toInt();

        lastBlock = payload;
        return lastBlock;
    }
    else
    {
        Serial.printf("HTTP GET request mempool block failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return 0;
}

String getMempoolFees()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(mempoolFeeApiUrl);

    int httpCode = http.GET();

    if (httpCode == 200)
    {
        char feeString[20];
        String payload = http.getString();
        JsonDocument doc;
        deserializeJson(doc, payload);

        snprintf(feeString, 20, "L: %d M: %d H: %d", doc["hourFee"].as<uint>(), doc["halfHourFee"].as<uint>(), doc["fastestFee"].as<uint>());

        return feeString;

        // preferences.putUInt("lastPrice", eurPrice);
    }
    else
    {
        Serial.printf("HTTP GET request mempool fees failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return "";
}