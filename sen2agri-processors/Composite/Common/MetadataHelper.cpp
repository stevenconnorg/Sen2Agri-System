#include "MetadataHelper.h"
#include <time.h>
#include <ctime>
#include <cmath>

MetadataHelper::MetadataHelper()
{
    m_nResolution = -1;
}

MetadataHelper::~MetadataHelper()
{

}

bool MetadataHelper::LoadMetadataFile(const std::string& file, int nResolution)
{
    Reset();
    m_inputMetadataFileName = file;
    m_nResolution = nResolution;
    return DoLoadMetadata();
}

void MetadataHelper::Reset()
{
    m_Mission = "";

    m_AotFileName = "";
    m_CloudFileName = "";
    m_WaterFileName = "";
    m_SnowFileName = "";
    m_ImageFileName = "";
    m_AcquisitionDate = "";

    m_ReflQuantifVal = 1;

    m_fAotQuantificationValue = 0.0;
    m_fAotNoDataVal = 0;
    m_nAotBandIndex = -1;

    m_nRedBandIndex = -1;
    m_nGreenBandIndex = -1;
    m_nNirBandIndex = -1;

    m_solarMeanAngles.azimuth = m_solarMeanAngles.zenith = 0.0;
}

int MetadataHelper::GetAcquisitionDateAsDoy()
{
    struct tm tmDate = {};
    if (strptime(m_AcquisitionDate.c_str(), "%Y%m%d", &tmDate) == NULL) {
        return -1;
    }
    auto curTime = std::mktime(&tmDate);

    std::tm tmYearStart = {};
    tmYearStart.tm_year = tmDate.tm_year;
    tmYearStart.tm_mon = 0;
    tmYearStart.tm_mday = 1;

    auto yearStart = std::mktime(&tmYearStart);
    auto diff = curTime - yearStart;

    return lrintf(diff / 86400 /* 60*60*24*/);
}

MeanAngles_Type MetadataHelper::GetSensorMeanAngles() {
    MeanAngles_Type angles;

    if(HasBandMeanAngles()) {
        int nBandsCnt = m_sensorBandsMeanAngles.size();
        for(size_t i = 0; i < m_sensorBandsMeanAngles.size(); i++) {
            MeanAngles_Type bandAngles = m_sensorBandsMeanAngles[i];
            angles.azimuth += bandAngles.azimuth;
            angles.zenith += bandAngles.zenith;
        }
        angles.azimuth = angles.azimuth / nBandsCnt;
        angles.zenith = angles.zenith / nBandsCnt;
    } else {
        angles = GetSensorMeanAngles(0);
    }

    return angles;
}

MeanAngles_Type MetadataHelper::GetSensorMeanAngles(int nBand) {
    MeanAngles_Type angles;
    angles.azimuth = 0;
    angles.zenith = 0;
    if(nBand >= 0 && nBand < (int)m_sensorBandsMeanAngles.size()) {
        angles = m_sensorBandsMeanAngles[nBand];
    } else if(m_sensorBandsMeanAngles.size() > 0) {
        angles = m_sensorBandsMeanAngles[0];
    }
    return angles;
}

double MetadataHelper::GetRelativeAzimuthAngle()
{
    MeanAngles_Type solarAngle = GetSolarMeanAngles();
    MeanAngles_Type sensorAngle = GetSensorMeanAngles();

    double relAzimuth = sensorAngle.azimuth - 180.0 - solarAngle.azimuth;

    if (relAzimuth < -180.0) {
        relAzimuth = relAzimuth + 360.0;
    }
    if (relAzimuth > 180.0) {
        relAzimuth = relAzimuth - 360.0;
    }

    return relAzimuth;
}
