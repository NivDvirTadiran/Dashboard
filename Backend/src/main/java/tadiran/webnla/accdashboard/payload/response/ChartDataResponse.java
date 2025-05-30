package tadiran.webnla.accdashboard.payload.response;

import java.util.List;

public class ChartDataResponse {
    private String type;
    private String title;
    private List<String> labels;
    private List<ChartDataset> datasets;

    public ChartDataResponse() {}

    public ChartDataResponse(String type, String title, List<String> labels, List<ChartDataset> datasets) {
        this.type = type;
        this.title = title;
        this.labels = labels;
        this.datasets = datasets;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public List<String> getLabels() {
        return labels;
    }

    public void setLabels(List<String> labels) {
        this.labels = labels;
    }

    public List<ChartDataset> getDatasets() {
        return datasets;
    }

    public void setDatasets(List<ChartDataset> datasets) {
        this.datasets = datasets;
    }

    public static class ChartDataset {
        private String label;
        private List<Double> data;
        private List<String> backgroundColor;
        private List<String> borderColor;
        private Integer borderWidth;

        public ChartDataset() {}

        public ChartDataset(String label, List<Double> data, List<String> backgroundColor) {
            this.label = label;
            this.data = data;
            this.backgroundColor = backgroundColor;
        }

        public String getLabel() {
            return label;
        }

        public void setLabel(String label) {
            this.label = label;
        }

        public List<Double> getData() {
            return data;
        }

        public void setData(List<Double> data) {
            this.data = data;
        }

        public List<String> getBackgroundColor() {
            return backgroundColor;
        }

        public void setBackgroundColor(List<String> backgroundColor) {
            this.backgroundColor = backgroundColor;
        }

        public List<String> getBorderColor() {
            return borderColor;
        }

        public void setBorderColor(List<String> borderColor) {
            this.borderColor = borderColor;
        }

        public Integer getBorderWidth() {
            return borderWidth;
        }

        public void setBorderWidth(Integer borderWidth) {
            this.borderWidth = borderWidth;
        }
    }
}