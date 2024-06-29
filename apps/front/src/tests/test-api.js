import { Box, Button, IconButton, Typography, useTheme } from "@mui/material";
import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { tokens } from "../../theme";
import { mockTransactions } from "../../data/mockData";
import DownloadOutlinedIcon from "@mui/icons-material/DownloadOutlined";
import Header from "../../components/Header";
import LineChart from "../../components/LineChart";
import GeographyChart from "../../components/GeographyChart";
import KeyboardArrowDownIcon from "@mui/icons-material/KeyboardArrowDown";

const Dashboard = () => {
  const theme = useTheme();
  const colors = tokens(theme.palette.mode);

  const [mediaData, setMediaData] = useState(null);
  const [realizaData, setRealizaData] = useState(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const mediaResponse = await axios.get('http://localhost:3333/media');
        const realizaResponse = await axios.get('http://localhost:3333/dados');
        setMediaData(mediaResponse.data[0]);
        setRealizaData(realizaResponse.data[0]);
      } catch (error) {
        console.error('Erro ao buscar dados', error);
      }
    };

    fetchData();
  }, []);

  return (
    <Box m="20px">
      {/* CABEÇALHO */}
      <Header title="DASHBOARD" subtitle="Bem-vindo ao seu painel de controle" />
      
      <Box display="flex" justifyContent="space-between" alignItems="center">
        <Button
          variant="contained"
          color="primary"
          startIcon={<DownloadOutlinedIcon />}
        >
          Baixar Relatórios
        </Button>
        <Box>
          <Typography variant="h6" color="textSecondary">
            Dados de Média: {mediaData ? mediaData.someField : 'Carregando...'}
          </Typography>
          <Typography variant="h6" color="textSecondary">
            Dados de Realização: {realizaData ? realizaData.someField : 'Carregando...'}
          </Typography>
        </Box>
      </Box>
      
      {/* Trajetórias */}
      <Box mt="20px">
        <Button
          variant="outlined"
          color="primary"
          endIcon={<KeyboardArrowDownIcon />}
        >
          Trajetórias Anteriores
        </Button>
      </Box>
      
      {/* Gráficos */}
      <Box mt="20px">
        <LineChart data={mockTransactions} />
        <GeographyChart />
      </Box>
    </Box>
  );
};

export default Dashboard;