/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tadiran.webnla.servlet;

//import com.orsoncharts.Chart3D;
//import com.orsoncharts.Chart3DFactory;
//import com.orsoncharts.Colors;
//import com.orsoncharts.TitleAnchor;
//import com.orsoncharts.data.PieDataset3D;
//import com.orsoncharts.data.StandardPieDataset3D;
//import com.orsoncharts.label.StandardPieLabelGenerator;
//import static com.orsoncharts.label.StandardPieLabelGenerator.PERCENT_TEMPLATE;
//import com.orsoncharts.legend.LegendAnchor;
//import com.orsoncharts.plot.PiePlot3D;
//import com.orsoncharts.util.Orientation;


import tadiran.webnla.NLAContext;
import tadiran.webnla.bean.AbsDataClass;
import tadiran.webnla.bean.DataItem;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Paint;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.HashMap;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.labels.StandardPieSectionLabelGenerator;
import org.jfree.chart.labels.StandardPieToolTipGenerator;
import org.jfree.chart.plot.PieLabelLinkStyle;
import org.jfree.chart.plot.PiePlot3D;
import org.jfree.data.general.DefaultPieDataset;
import org.jfree.data.general.PieDataset;

import static org.jfree.chart.ChartUtils.writeChartAsPNG;

//import org.krysalis.jcharts.chartData.ChartDataException;
//import org.krysalis.jcharts.chartData.PieChartDataSet;
//import org.krysalis.jcharts.types.PieLabelType;

/**
 *
 * @author a180042
 */
public class JFreeChartDrawChart extends HttpServlet {

        /**
         * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
         * methods.
         *
         * @param request servlet request
         * @param response servlet response
         * @throws ServletException if a servlet-specific error occurs
         * @throws IOException if an I/O error occurs
         */
        private static final Logger  log = LogManager.getLogger("NLAData");
        private transient NLAContext ctx = NLAContext.getInstance();
        private static final Paint[] availableAltPaints = {new Color(200,200,200,220), new Color(250,250,250,220)};
        private static final Color[] availablePaints = {new Color(65,105,225,200),  //royal blue        #4169E1     (65,105,225)    old - new Color(0,0,255,200) - Blue
                                                        new Color(152,251,152,200), //pale green	#98FB98     (152,251,152)   old - new Color(0,200,0,200) - Green
                                                        new Color(199,21,133,200),  //medium violet red	#C71585     (199,21,133)    old - new Color(255,0,0,200) - RED
                                                        new Color(218,165,32),      //golden rod	#DAA520     (218,165,32)    old - new Color(255,255,0)   - Yellow
                                                        new Color(255,182,193),     //light pink	#FFB6C1     (255,182,193)   old - new Color(255,0,255)   - Magenta
                                                        new Color(250,250,250),     //White
                                                        new Color(60,179,113),      //medium sea green	#3CB371     (60,179,113)    old - new Color(0,115,2)     - Dark Green
                                                        new Color(206,75,2),
                                                        new Color(150,150,2),
                                                        new Color(2,0,115),
                                                        new Color(2,150,206),
                                                        new Color(255,130,130),
                                                        new Color(115,2,0),
                                                        new Color(75,2,206),
                                                        new Color(115,2,75)};

        
	private Paint [] getColors(int[] map) 
        {
                Paint[] colors;
                int size=map.length;
		if (size>0) 
                {
                        colors=new Paint[size];
                        for (int i=0; i<size; i++) {
                                colors[map[i]]=availablePaints[i];
                        }
		}
                else 
                {
                        int ind=-size-1;
                        colors=new Paint[-size];
                        for (int i=0; i<-size; i++) {
                                colors[i]=availablePaints[ind-i];
                        }
                }

                return colors;
        }


        private Dimension getSize(String size, Dimension defaultSize)
        {
                if (size==null)
                        return defaultSize;
                else
                {
                        String[] dim=size.split("x");
                        return new Dimension(Integer.parseInt(dim[0]),Integer.parseInt(dim[1]));
                }
        }

    
        private PieDataset createPieChartDataset3D(HttpServletRequest request) 
        {
                String[] fields = (String[])request.getAttribute("fields");
                DefaultPieDataset dataset = new DefaultPieDataset( );

                AbsDataClass obj = ctx.getObject(request.getParameter("refObjectName"));
                HashMap<String,DataItem> objData = obj.getData();
                DataItem item;
                double sum = 0;
                
                for(int i=0; i<fields.length; i++) 
                {
                        item = objData.get(fields[i]);
                        if (item == null) {
                                item = new DataItem(fields[i],"...",0);
			}
                        
                        //dataset.setValue( item.getName() , Double.valueOf(item.getValue().toString()));
                }

//                Paint[] colors=getColors(map);
//                if (sum == 0) 
//                {
//                        dataset.setValue( "no data" , 0.1); 
//                        if (fields.length<6) 
//                        {
//                                data=new double[data.length+1]; data[data.length-1]=0.1;
//                                colors=Arrays.copyOf(colors, colors.length+1); colors[colors.length-1]=availablePaints[5];
//                                labels=Arrays.copyOf(labels,labels.length+1); labels[labels.length-1]="no data";
//                        }
//                        else {
//                                data[map[5]]=0.1;
//                        }
//                        pieChart3DProperties.setPieLabelType(PieLabelType.NO_LABELS);
//                }
//                else {
//                        pieChart3DProperties.setPieLabelType(PieLabelType.VALUE_LABELS);
//                }

                return dataset;
                //return new PieChartDataSet(title, data, labels, colors, pieChart3DProperties);
        }

        
//        private JFreeChart createPieChart3D(HttpServletRequest request)
//        {
//                String title=(String)request.getAttribute("title");
//                PieDataset dataset = createPieChartDataset3D(request);
//
//                JFreeChart chart = ChartFactory.createPieChart3D( title,   // chart title
//                                                                  dataset, // data
//                                                                  true,    // include legend
//                                                                  true,
//                                                                  false);
//
//                final PiePlot3D plot = ( PiePlot3D ) chart.getPlot( );
//
//                for(int i=0; i<dataset.getItemCount(); i++)
//                {
//                    plot.setSectionPaint(dataset.getKey(i), availablePaints[i]);
//                    //plot.setSectionPaint(i, availablePaints[i]);
//
//                }
//                plot.setStartAngle( 270 );
//                plot.setForegroundAlpha( 0.60f );
//                plot.setInteriorGap( 0.02 );
//                //plot.setBackgroundPaint(Color.YELLOW);
//                //plot.setOutlinePaint(Color.BLACK);
//                plot.setLabelGenerator(new StandardPieSectionLabelGenerator("{0}({1})--{2}"));
//                plot.setToolTipGenerator(new StandardPieToolTipGenerator());
//                plot.setLabelLinkStyle(PieLabelLinkStyle.STANDARD);
//
//                // Customize no data
//                plot.setNoDataMessage("No data to display");
//
//                return chart;
//        }

        
        @Override
        public void init () throws ServletException {
            super.init();
//            pieChart3DProperties.setPieLabelType(PieLabelType.VALUE_LABELS);
//            pieChart3DProperties.setZeroDegreeOffset(90);
//            pieChart3DProperties.setValueLabelFont(ChartFont.DEFAULT_AXIS_VALUE);
//            pieChart3DProperties.setTickLength(4f);
//            pieChart3DProperties.setBorderChartStroke(new ChartStroke(new BasicStroke(1.2f), new Color(240,240,240)));
//            //new BasicStroke(0.5f), Color.GRAY));
//
//            legendProperties.setPlacement(LegendProperties.RIGHT);
//            legendProperties.setChartPadding(25);
//            legendProperties.setNumColumns(1);
//            //legendProperties.setChartFont(new ChartFont(new Font(legendProperties.getChartFont().getFont().getFontName(),Font.PLAIN, 11), Color.black));
//            legendProperties.setBorderStroke(null);
//
//            chartProperties.setEdgePadding(0);
//            chartProperties.setTitlePadding(0);
//            chartProperties.setValidate(false);
//
//            //axisProperties.setXAxisLabelsAreVertical(true);
//            AxisTypeProperties xAxisProps=axisProperties.getXAxisProperties();
//            axisProperties.setBackgroundPaint(new Color(245,245,245,200));
//            ChartFont xScaleChartFont= new ChartFont( new Font(xAxisProps.getScaleChartFont().getFont().getFontName(), Font.PLAIN, 9 ), Color.black );
//            xAxisProps.setScaleChartFont( xScaleChartFont );
//            //xAxisProps.setShowGridLines(AxisTypeProperties.GRID_LINES_ALL);
//            xAxisProps.setShowEndBorder(false);
//            xAxisProps.setPaddingBetweenXAxisLabels(0);
//
//            DataAxisProperties dataAxisProperties= (DataAxisProperties) axisProperties.getYAxisProperties();
//            dataAxisProperties.setNumItems(10);
//            dataAxisProperties.setShowGridLines(AxisTypeProperties.GRID_LINES_ONLY_WITH_LABELS);
//            dataAxisProperties.setShowEndBorder(false);

        }
    
    
        protected void processRequest(HttpServletRequest request, HttpServletResponse response)
                throws ServletException, IOException {
            response.setContentType("text/html;charset=UTF-8");
            try (PrintWriter out = response.getWriter()) {
                /* TODO output your page here. You may use following sample code. */
                out.println("<!DOCTYPE html>");
                out.println("<html>");
                out.println("<head>");
                out.println("<title>Servlet OrsonChartDrawChart</title>");            
                out.println("</head>");
                out.println("<body>");
                out.println("<h1>Servlet OrsonChartDrawChart at " + request.getContextPath() + "</h1>");
                out.println("</body>");
                out.println("</html>");
            }
        }

        // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
        /**
         * Handles the HTTP <code>GET</code> method.
         *
         * @param request servlet request
         * @param response servlet response
         * @throws ServletException if a servlet-specific error occurs
         * @throws IOException if an I/O error occurs
         */
        @Override
        protected void doGet(HttpServletRequest request, HttpServletResponse response)
                throws ServletException, IOException 
        {
                request.getSession(true);
                JFreeChart chart;

                // prepare common chart params
                String title=request.getParameter("title");
                if ("".equals(title))
                    title=null;
                request.setAttribute("title",title);

                String keys=request.getParameter("keys");
                if (keys==null)
                        return;
                request.setAttribute("fields",keys.split("[ ]*,[ ]*"));

                String size=request.getParameter("size");
                if ("".equals(size))
                    size=null;


                // create chart by request
                try {
                        Dimension chartSize;
                        chart=null;
                        String type=request.getParameter("type");
                        log.debug("type="+type+", keys="+keys+", size="+size);

                        if (type.equals("bar") && keys.indexOf("-")>0)
                                type="bar-stack";


                        if (type.equals("3Dpie"))
                        {
                               // chart = createPieChart3D(request);
//                                PieDataset pieChartDataSet = get3DPieChartDataSet(request);
//                                chart= new PieChart3D( pieChartDataSet, legendProperties, chartProperties, chartSize.width, chartSize.height);
                        }

			if (chart!=null)
                        {
                                chartSize = getSize(size,new Dimension(300,200));
                                //ServletEncoderHelper.encodeJPEG13(chart, 0.75f, response);
                                //ServletEncoderHelper.encodePNG(chart, response);
                                writeChartAsPNG(response.getOutputStream(), chart, chartSize.width, chartSize.height );
                                writeChartAsPNG(response.getOutputStream(), chart, 700, 400);
                        }
                }

                catch (Exception ex) {
                        log.error("Cannot create chart:"+ex);
                        //if (log.isDebugEnabled())
                        ex.printStackTrace();
                }

                //processRequest(request, response);
        }

        /**
         * Handles the HTTP <code>POST</code> method.
         *
         * @param request servlet request
         * @param response servlet response
         * @throws ServletException if a servlet-specific error occurs
         * @throws IOException if an I/O error occurs
         */
        @Override
        protected void doPost(HttpServletRequest request, HttpServletResponse response)
                throws ServletException, IOException {
            processRequest(request, response);
        }

        /**
         * Returns a short description of the servlet.
         *
         * @return a String containing servlet description
         */
        @Override
        public String getServletInfo() {
            return "Short description";
        }// </editor-fold>

    
}
