#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include "otbWrapperTypes.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"

class GaussianFilter
{
public:
    typedef otb::Wrapper::FloatImageType ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    typedef otb::ImageFileWriter<ImageType> WriterType;

    typedef itk::RescaleIntensityImageFilter<
        ImageType, ImageType> RescaleFilterType;

    typedef itk::DiscreteGaussianImageFilter<
          ImageType, ImageType>  DiscreteGaussianFilterType;

    typedef itk::ImageSource<ImageType> ImageSource;
    typedef DiscreteGaussianFilterType::Superclass::Superclass OutImageSource;

public:
    GaussianFilter();

    void SetInputFileName(std::string &inputImageStr);
    void SetOutputFileName(std::string &outFile);
    void SetInputImageReader(ImageSource::Pointer inputReader);
    void SetSigma(float fSigma);

    const char* GetNameOfClass() { return "GaussianFilter"; }
    OutImageSource::Pointer GetOutputImageSource();
    void WriteToOutputFile();

private:
    void BuildOutputImageSource();
    float m_fSigma;
    std::string m_outputFileName;
    RescaleFilterType::Pointer m_rescaler;
    DiscreteGaussianFilterType::Pointer m_gaussianFilter;
    ImageSource::Pointer m_inputReader;
};

#endif // GAUSSIANFILTER_H
