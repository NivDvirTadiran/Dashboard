package tadiran.webnla.accdashboard.services;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.stereotype.Service;
import tadiran.webnla.NLAContext;
import tadiran.webnla.accdashboard.payload.response.ChartDataResponse;
import tadiran.webnla.accdashboard.payload.response.ChartDataResponse.ChartDataset;
import tadiran.webnla.bean.AbsDataClass;
import tadiran.webnla.bean.DataItem;
import tadiran.webnla.tag.LocaleMessage;

import jakarta.servlet.http.HttpServletRequest;
import java.awt.*;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.*;
import java.util.List;

@Service
public class ChartDataService {

    private static final Logger logger = LogManager.getLogger(ChartDataService.class);

    private transient NLAContext ctx = NLAContext.getInstance();

    private static final Color[] availablePaints = {
            new Color(51, 153, 255),    // Light Blue
            new Color(0, 255, 51),      // Light Green
            new Color(255, 51, 51),     // Light Red
            new Color(255, 204, 0),     // Dark Yellow
            new Color(255, 0, 255),     // Magenta
            new Color(153, 153, 153),   // Gray
            new Color(153, 102, 0),     // Light Brown
            new Color(255, 153, 0),     // Light Orange
            new Color(150, 150, 2),
            new Color(2, 0, 115),
            new Color(2, 150, 206),
            new Color(255, 130, 130),
            new Color(115, 2, 0),
            new Color(75, 2, 206),
            new Color(115, 2, 75)
    };

    public ChartDataResponse getChartData(HttpServletRequest request, String type, String title, String keys, String refObjectName) {
        try {
            logger.debug("ChartDataService - getChartData: Chart Type {}, keys {}, refObjectName: {}", type, keys, refObjectName);

            if (keys == null) {
                throw new IllegalArgumentException("Keys parameter is required");
            }

            request.getSession(true);

            if (title != null && title.isEmpty()) {
                title = null;
            }

            String[] fields = keys.split("[ ]*,[ ]*");

            if (type == null) {
                type = "3Dpie"; // Default to pie chart
            }

            if (type.equals("3Dpie") || type.equals("pie")) {
                return getPieChartData(request, title, fields, refObjectName);
            } else {
                return getBarLineAreaChartData(request, type, title, fields, refObjectName);
            }

        } catch (Exception e) {
            logger.error("Error generating chart data", e);
            throw new RuntimeException("Failed to generate chart data", e);
        }
    }

    private ChartDataResponse getPieChartData(HttpServletRequest request, String title, String[] fields, String refObjectName)
            throws UnsupportedEncodingException, InterruptedException {
        if (refObjectName == null) {
            throw new IllegalArgumentException("refObjectName parameter is required for pie chart data generation");
        }

        String langName = (String) request.getSession().getAttribute("Accept-Language");

        AbsDataClass obj = ctx.getObject(refObjectName);
        if (obj == null) {
            throw new IllegalArgumentException("Data object not found for refObjectName: " + refObjectName);
        }

        HashMap<String, DataItem> objData = obj.getData();
        if (objData == null) {
            throw new IllegalArgumentException("Data map is null for refObjectName: " + refObjectName);
        }

        List<Double> data = new ArrayList<>();
        List<String> labels = new ArrayList<>();
        List<String> backgroundColors = new ArrayList<>();

        double sum = 0;
        for (int i = 0; i < fields.length; i++) {
            DataItem item = objData.get(fields[i]);
            if (item == null) {
                item = new DataItem(fields[i], "...", 0);
            }
            double val = Double.valueOf(item.getValue().toString());
            
            if (val != 0) {
                sum += val;
                data.add(val);
                labels.add(item.getName(langName));
                backgroundColors.add(colorToHex(availablePaints[i % availablePaints.length]));
            }
        }

        // Handle case where all values are 0
        if (sum == 0) {
            data.add(0.1);
            labels.add("no data");
            backgroundColors.add(colorToHex(availablePaints[5]));
        }

        String decodedTitle = null;
        if (title != null) {
            decodedTitle = URLDecoder.decode(title, "UTF-8");
        }

        ChartDataset dataset = new ChartDataset("", data, backgroundColors);
        List<ChartDataset> datasets = Arrays.asList(dataset);

        return new ChartDataResponse("pie", decodedTitle, labels, datasets);
    }

    private ChartDataResponse getBarLineAreaChartData(HttpServletRequest request, String type, String title, String[] fields, String refObjectName)
            throws UnsupportedEncodingException, InterruptedException {
        if (refObjectName == null) {
            throw new IllegalArgumentException("refObjectName parameter is required for chart data generation");
        }

        String langName = (String) request.getSession().getAttribute("Accept-Language");
        String[] legendLabels = refObjectName.split("[ ]*,[ ]*");

        // Handle stacked bar charts
        if (type.equals("bar") && fields[0].indexOf("-") > 0) {
            type = "bar-stack";
        }

        String[] fields0 = fields[0].split("-");
        if (fields0.length > legendLabels.length) {
            String prevLegendLabel = legendLabels[0];
            legendLabels = new String[fields0.length];
            legendLabels[0] = prevLegendLabel;
            for (int i = 1; i < fields0.length; i++) {
                legendLabels[i] = legendLabels[0];
            }
        }

        List<String> labels = new ArrayList<>();
        double[][] data = new double[legendLabels.length][fields.length];

        // Extract data from objects
        for (int r = 0; r < legendLabels.length; r++) {
            AbsDataClass obj = ctx.getObject(legendLabels[r]);
            if (obj == null) continue;
            
            HashMap<String, DataItem> objData = obj.getData();
            String objName = obj.getName();
            if (objName != null && !objName.equals(legendLabels[r])) {
                legendLabels[r] = objName;
            }

            for (int i = 0; i < fields.length; i++) {
                String[] field = fields[i].split("-");
                String ersKey = "null";
                if (field.length == 1) {
                    ersKey = fields[i];
                } else {
                    if (r < field.length) {
                        ersKey = field[r];
                    }
                }

                DataItem item = objData.get(ersKey);
                String itemName = "null";
                if (item == null) {
                    item = new DataItem(ersKey, (ersKey.length() > 0) ? ersKey : itemName, 0);
                } else {
                    if (!(field.length == 1 && r > 0)) {
                        itemName = item.getName(langName);
                    }
                }

                data[r][i] = Double.valueOf(item.getValue().toString());

                if (r == 0) {
                    labels.add(itemName);
                }
            }
        }

        String decodedTitle = null;
        if (title != null) {
            decodedTitle = URLDecoder.decode(title, "UTF-8");
        }

        // Create datasets
        List<ChartDataset> datasets = new ArrayList<>();
        for (int r = 0; r < legendLabels.length; r++) {
            List<Double> dataList = new ArrayList<>();
            for (int i = 0; i < fields.length; i++) {
                dataList.add(data[r][i]);
            }

            List<String> backgroundColor = Arrays.asList(colorToHex(availablePaints[r % availablePaints.length]));
            ChartDataset dataset = new ChartDataset(legendLabels[r], dataList, backgroundColor);
            datasets.add(dataset);
        }

        // Convert chart type to Chart.js compatible type
        String chartJsType = convertToChartJsType(type);

        return new ChartDataResponse(chartJsType, decodedTitle, labels, datasets);
    }

    private String convertToChartJsType(String type) {
        switch (type) {
            case "3Dpie":
                return "pie";
            case "bar-stack":
                return "bar";
            case "area":
                return "line"; // Chart.js uses line with fill option for area charts
            default:
                return type;
        }
    }

    private String colorToHex(Color color) {
        return String.format("#%02x%02x%02x", color.getRed(), color.getGreen(), color.getBlue());
    }
}