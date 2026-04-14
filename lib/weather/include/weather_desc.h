#pragma once

#include <string_view>
namespace weather {

enum class WeatherCode {
  None                          = 000,

  // Group 2xx: Thunderstorm
  Thunderstorm_LightRain        = 200,
  Thunderstorm_Rain             = 201,
  Thunderstorm_HeavyRain        = 202,
  Thunderstorm_Light            = 210,
  Thunderstorm_Normal           = 211,
  Thunderstorm_Heavy            = 212,
  Thunderstorm_Ragged           = 221,
  Thunderstorm_LightDrizzle     = 230,
  Thunderstorm_Drizzle          = 231,
  Thunderstorm_HeavyDrizzle     = 232,

  // Group 3xx: Drizzle
  Drizzle_LightIntensity        = 300,
  Drizzle_Normal                = 301,
  Drizzle_HeavyIntensity        = 302,
  Drizzle_LightRain             = 310,
  Drizzle_Rain                  = 311,
  Drizzle_HeavyRain             = 312,
  Drizzle_ShowerRain            = 313,
  Drizzle_HeavyShowerRain       = 314,
  Drizzle_Shower                = 321,

  // Group 5xx: Rain
  Rain_Light                    = 500,
  Rain_Moderate                 = 501,
  Rain_HeavyIntensity           = 502,
  Rain_VeryHeavy                = 503,
  Rain_Extreme                  = 504,
  Rain_Freezing                 = 511,
  Rain_LightShower              = 520,
  Rain_Shower                   = 521,
  Rain_HeavyShower              = 522,
  Rain_RaggedShower             = 531,

  // Group 6xx: Snow
  Snow_Light                    = 600,
  Snow_Normal                   = 601,
  Snow_Heavy                    = 602,
  Snow_Sleet                    = 611,
  Snow_LightShowerSleet         = 612,
  Snow_ShowerSleet              = 613,
  Snow_LightRainAndSnow         = 615,
  Snow_RainAndSnow              = 616,
  Snow_LightShower              = 620,
  Snow_Shower                   = 621,
  Snow_HeavyShower              = 622,

  // Group 7xx: Atmosphere
  Atmosphere_Mist               = 701,
  Atmosphere_Smoke              = 711,
  Atmosphere_Haze               = 721,
  Atmosphere_DustWhirls         = 731,
  Atmosphere_Fog                = 741,
  Atmosphere_Sand               = 751,
  Atmosphere_Dust               = 761,
  Atmosphere_Ash                = 762,
  Atmosphere_Squall             = 771,
  Atmosphere_Tornado            = 781,

  // Group 800: Clear
  Clear_Sky                     = 800,

  // Group 80x: Clouds
  Clouds_Few                    = 801,
  Clouds_Scattered              = 802,
  Clouds_Broken                 = 803,
  Clouds_Overcast               = 804
};

inline constexpr std::string_view to_string(WeatherCode code) {
  switch (code) {

    // 2xx — Burze
    case WeatherCode::Thunderstorm_LightRain:        return "Burza z lekkim deszczem";
    case WeatherCode::Thunderstorm_Rain:             return "Burza z deszczem";
    case WeatherCode::Thunderstorm_HeavyRain:        return "Burza z ulewnym deszczem";
    case WeatherCode::Thunderstorm_Light:            return "Lekka burza";
    case WeatherCode::Thunderstorm_Normal:           return "Burza";
    case WeatherCode::Thunderstorm_Heavy:            return "Silna burza";
    case WeatherCode::Thunderstorm_Ragged:           return "Burza z poszarpanymi wyladowaniami";
    case WeatherCode::Thunderstorm_LightDrizzle:     return "Burza z lekka mzawka";
    case WeatherCode::Thunderstorm_Drizzle:          return "Burza z mzawka";
    case WeatherCode::Thunderstorm_HeavyDrizzle:     return "Burza z intensywna mzawka";

    // 3xx — Mzawka
    case WeatherCode::Drizzle_LightIntensity:        return "Lekka mzawka";
    case WeatherCode::Drizzle_Normal:                return "Mzawka";
    case WeatherCode::Drizzle_HeavyIntensity:        return "Intensywna mzawka";
    case WeatherCode::Drizzle_LightRain:             return "Lekka mzawka z deszczem";
    case WeatherCode::Drizzle_Rain:                  return "Mzawka z deszczem";
    case WeatherCode::Drizzle_HeavyRain:             return "Intensywna mzawka z deszczem";
    case WeatherCode::Drizzle_ShowerRain:            return "Przelotna mzawka";
    case WeatherCode::Drizzle_HeavyShowerRain:       return "Intensywna przelotna mzawka";
    case WeatherCode::Drizzle_Shower:                return "Przelotna mzawka";

    // 5xx — Deszcz
    case WeatherCode::Rain_Light:                    return "Lekki deszcz";
    case WeatherCode::Rain_Moderate:                 return "Umiarkowany deszcz";
    case WeatherCode::Rain_HeavyIntensity:           return "Intensywny deszcz";
    case WeatherCode::Rain_VeryHeavy:                return "Bardzo intensywny deszcz";
    case WeatherCode::Rain_Extreme:                  return "Ekstremalny deszcz";
    case WeatherCode::Rain_Freezing:                 return "Marznacy deszcz";
    case WeatherCode::Rain_LightShower:              return "Lekki przelotny deszcz";
    case WeatherCode::Rain_Shower:                   return "Przelotny deszcz";
    case WeatherCode::Rain_HeavyShower:              return "Ulewny przelotny deszcz";
    case WeatherCode::Rain_RaggedShower:             return "Nieregularne przelotne opady";

    // 6xx — Snieg
    case WeatherCode::Snow_Light:                    return "Lekki snieg";
    case WeatherCode::Snow_Normal:                   return "Snieg";
    case WeatherCode::Snow_Heavy:                    return "Intensywny snieg";
    case WeatherCode::Snow_Sleet:                    return "Deszcz ze sniegiem";
    case WeatherCode::Snow_LightShowerSleet:         return "Lekki przelotny deszcz ze sniegiem";
    case WeatherCode::Snow_ShowerSleet:              return "Przelotny deszcz ze sniegiem";
    case WeatherCode::Snow_LightRainAndSnow:         return "Lekki deszcz i snieg";
    case WeatherCode::Snow_RainAndSnow:              return "Deszcz i snieg";
    case WeatherCode::Snow_LightShower:              return "Lekki przelotny snieg";
    case WeatherCode::Snow_Shower:                   return "Przelotny snieg";
    case WeatherCode::Snow_HeavyShower:              return "Intensywny przelotny snieg";

    // 7xx — Atmosfera
    case WeatherCode::Atmosphere_Mist:               return "Mgla";
    case WeatherCode::Atmosphere_Smoke:              return "Dym";
    case WeatherCode::Atmosphere_Haze:               return "Zamglenie";
    case WeatherCode::Atmosphere_DustWhirls:         return "Zawirowania pylu";
    case WeatherCode::Atmosphere_Fog:                return "Gesta mgla";
    case WeatherCode::Atmosphere_Sand:               return "Piasek";
    case WeatherCode::Atmosphere_Dust:               return "Pyl";
    case WeatherCode::Atmosphere_Ash:                return "Popiol";
    case WeatherCode::Atmosphere_Squall:             return "Szkwal";
    case WeatherCode::Atmosphere_Tornado:            return "Tornado";

    // 800 — Bezchmurnie
    case WeatherCode::Clear_Sky:                     return "Bezchmurnie";

    // 80x — Chmury
    case WeatherCode::Clouds_Few:                    return "Male zachmurzenie";
    case WeatherCode::Clouds_Scattered:              return "Rozproszone chmury";
    case WeatherCode::Clouds_Broken:                 return "Duze zachmurzenie";
    case WeatherCode::Clouds_Overcast:               return "Zachmurzenie calkowite";

    default:                                         return "";
  }
}

}
